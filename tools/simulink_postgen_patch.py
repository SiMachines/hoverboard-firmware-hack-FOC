#!/usr/bin/env python3
"""Post-process Simulink-generated BLDC_controller files.

This repo intentionally wraps Simulink-generated sources instead of forking them.
When you re-generate the model, Simulink may revert a few project-specific
integration tweaks. This script re-applies those tweaks automatically.

What it fixes (idempotent):
- Adds RT_MODEL::defaultParam in Inc/BLDC_controller.h (if missing)
- Ensures Inc/BLDC_controller.h includes config.h when mcu_model is not yet defined
- Comments out global "extern P rtP;" declaration (we use per-instance params)
- Ensures BLDC_controller_step() / BLDC_controller_initialize() define a local
    param pointer:
        P *rtP = ((P *) rtM->defaultParam);
- Threads the per-instance `rtP` pointer through generated helper functions
    that incorrectly reference global params after regeneration
- Fixes accidental dot-access for parameter pointers: rtp.*/rtP.* -> rtp->*/rtP->*
- Renames the generated default parameter struct in Src/BLDC_controller_data.c:
    P rtP -> P rtP_Left

Usage (from repo root):
  python tools/simulink_postgen_patch.py

Exit status is non-zero on error.
"""

from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[1]


@dataclass
class FileEdit:
    path: Path
    changed: bool
    details: list[str]


def _detect_newline(text: str) -> str:
    return "\r\n" if "\r\n" in text else "\n"


def _read_text(path: Path) -> str:
    # newline='' avoids newline translation; keeps CRLF intact.
    return path.open("r", encoding="utf-8", newline="").read()


def _write_text(path: Path, text: str) -> None:
    path.open("w", encoding="utf-8", newline="").write(text)


def _patch_header(text: str) -> tuple[str, list[str]]:
    details: list[str] = []
    newline = _detect_newline(text)

    # Ensure generated header can resolve mcu_model regardless of include order.
    guarded_include = f"#ifndef mcu_model{newline}#include \"config.h\"{newline}#endif"
    if guarded_include not in text:
        # Locate the include block. New-style "portable word sizes" codegen
        # emits <stdbool.h>/<stdint.h>; older codegen emitted "rtwtypes.h".
        include_block_re = re.compile(
            r"(?:#include\s+<stdint\.h>\s*\r?\n|#include\s+\"rtwtypes\.h\"\s*\r?\n)",
            flags=re.MULTILINE,
        )
        m_inc = include_block_re.search(text)
        if not m_inc:
            raise RuntimeError("Could not find the include block in BLDC_controller.h")
        text = text[: m_inc.end()] + guarded_include + newline + text[m_inc.end() :]
        details.append("added guarded include for config.h (mcu_model)")

    # Ensure RT_MODEL has defaultParam pointer
    if "P *defaultParam;" not in text:
        pattern = re.compile(r"(struct\s+tag_RTM\s*\{\s*)")
        m = pattern.search(text)
        if not m:
            raise RuntimeError("Could not find 'struct tag_RTM {' in BLDC_controller.h")
        insert = f"  P *defaultParam;{newline}"
        text = text[: m.end(1)] + insert + text[m.end(1) :]
        details.append("added RT_MODEL::defaultParam")

    # Comment out global rtP declaration (if present and not already commented)
    def repl_extern(match: re.Match[str]) -> str:
        indent = match.group("indent")
        return f"{indent}//extern P rtP;"

    new_text, n = re.subn(
        r"^(?P<indent>\s*)extern\s+P\s+rtP\s*;\s*$",
        repl_extern,
        text,
        flags=re.MULTILINE,
    )
    if n:
        text = new_text
        details.append("commented out 'extern P rtP;' (use rtM->defaultParam)")

    return text, details


def _ensure_local_param_ptr(text: str, function_name: str) -> tuple[str, list[str]]:
    details: list[str] = []
    newline = _detect_newline(text)

    signature_re = re.compile(
        rf"^\s*void\s+{re.escape(function_name)}\s*\(\s*RT_MODEL\s*\*\s*const\s+rtM\s*\)\s*$"
    )
    lines = text.splitlines(keepends=True)

    # Find function signature line
    sig_idx = None
    for i, line in enumerate(lines):
        if signature_re.match(line.rstrip("\r\n")):
            sig_idx = i
            break
    if sig_idx is None:
        # Some generators put the brace on same line; fallback search.
        for i, line in enumerate(lines):
            if function_name in line and "RT_MODEL" in line and "rtM" in line:
                sig_idx = i
                break
    if sig_idx is None:
        # Function not present; nothing to do.
        return text, details

    # Find opening brace
    brace_idx = None
    for j in range(sig_idx, min(sig_idx + 10, len(lines))):
        if "{" in lines[j]:
            brace_idx = j
            break
    if brace_idx is None:
        raise RuntimeError(f"Could not locate opening '{{' for {function_name}()")

    needle = "P *rtP = ((P *) rtM->defaultParam);"

    # Check if already present shortly after brace
    for j in range(brace_idx, min(brace_idx + 25, len(lines))):
        if needle in lines[j]:
            return text, details

    # Insert after the opening brace line.
    insert_line = f"  {needle}{newline}"
    lines.insert(brace_idx + 1, insert_line)

    details.append(f"inserted local param pointer in {function_name}()")
    return "".join(lines), details


def _patch_param_access(text: str) -> tuple[str, list[str]]:
    details: list[str] = []

    # If the generator produced dot access on a pointer, fix it.
    # Keep it very narrow to avoid touching other symbols.
    new_text, n1 = re.subn(r"\brtP\.", "rtP->", text)
    new_text, n2 = re.subn(r"\brtp\.", "rtp->", new_text)

    if n1:
        details.append(f"replaced {n1} occurrence(s) of 'rtP.' with 'rtP->'")
    if n2:
        details.append(f"replaced {n2} occurrence(s) of 'rtp.' with 'rtp->'")

    return new_text, details


def _patch_generated_rtP_plumbing(text: str) -> tuple[str, list[str]]:
    details: list[str] = []

    if "rtP->" not in text:
        return text, details

    patterns: list[tuple[str, str, str]] = [
        (
            "updated F03_Control_Mode_Manager() prototype to accept rtP",
            r"(extern\s+void\s+F03_Control_Mode_Manager\([\s\S]*?\*rty_r_inpTgtConv,\s*)(?:const\s+P\s*\*\s*rtP,\s*)?(DW_F03_Control_Mode_Manager\s*\*localDW\);)",
            r"\1const P *rtP, \2",
        ),
        (
            "updated F03_Control_Mode_Manager() definition to accept rtP",
            r"(void\s+F03_Control_Mode_Manager\([\s\S]*?\*rty_r_inpTgtConv,\s*)(?:const\s+P\s*\*\s*rtP,\s*)?(DW_F03_Control_Mode_Manager\s*\*localDW\)\s*\{)",
            r"\1const P *rtP, \2",
        ),
        (
            "updated PI_clamp_fixdtFF() prototype to accept rtP",
            r"(extern\s+int16_T\s+PI_clamp_fixdtFF\([\s\S]*?rtu_FeedForward,\s*)(?:const\s+P\s*\*\s*rtP,\s*)?(DW_PI_clamp_fixdtFF\s*\*localDW\);)",
            r"\1const P *rtP, \2",
        ),
        (
            "updated PI_clamp_fixdtFF() definition to accept rtP",
            r"(int16_T\s+PI_clamp_fixdtFF\([\s\S]*?rtu_FeedForward,\s*)(?:const\s+P\s*\*\s*rtP,\s*)?(DW_PI_clamp_fixdtFF\s*\*localDW\)\s*\{)",
            r"\1const P *rtP, \2",
        ),
        (
            "updated PI_clamp_fixdtFF_c() prototype to accept rtP",
            r"(extern\s+int16_T\s+PI_clamp_fixdtFF_c\([\s\S]*?rtu_FeedForward,\s*)(?:const\s+P\s*\*\s*rtP,\s*)?(DW_PI_clamp_fixdtFF_k\s*\*localDW\);)",
            r"\1const P *rtP, \2",
        ),
        (
            "updated PI_clamp_fixdtFF_c() definition to accept rtP",
            r"(int16_T\s+PI_clamp_fixdtFF_c\([\s\S]*?rtu_FeedForward,\s*)(?:const\s+P\s*\*\s*rtP,\s*)?(DW_PI_clamp_fixdtFF_k\s*\*localDW\)\s*\{)",
            r"\1const P *rtP, \2",
        ),
        (
            "updated FOC() prototype to accept rtP",
            r"(extern\s+void\s+FOC\([\s\S]*?rtu_b_cruiseCtrlEna,\s*)(?:const\s+P\s*\*\s*rtP,\s*)?(int16_t\s*\*rty_Vq,\s*int16_t\s*\*rty_Vd,\s*DW_FOC[\s\S]*?localDW\);)",
            r"\1const P *rtP, \2",
        ),
        (
            "updated FOC() definition to accept rtP",
            r"(void\s+FOC\([\s\S]*?rtu_b_cruiseCtrlEna,\s*)(?:const\s+P\s*\*\s*rtP,\s*)?(int16_t\s*\*rty_Vq,\s*int16_t\s*\*rty_Vd,[\s\S]*?DW_FOC\s*\*localDW\)\s*\{)",
            r"\1const P *rtP, \2",
        ),
    ]

    for detail, pattern, replacement in patterns:
        new_text, count = re.subn(pattern, replacement, text, count=1, flags=re.MULTILINE)
        if count:
            text = new_text
            details.append(detail)

    new_text, count = re.subn(
        r"(rtu_b_cruiseCtrlEna,\s*)(?:const\s+P\s*\*\s*rtP,\s*)?(int16_t\s*\*rty_Vq,\s*int16_t\s*\*rty_Vd,\s*DW_FOC\s*\*localDW)",
        r"\1const P *rtP, \2",
        text,
        flags=re.MULTILINE,
    )
    if count:
        text = new_text
        details.append(f"normalized FOC() parameter list(s) to include rtP ({count})")

    call_patterns: list[tuple[str, str, str]] = [
        (
            "updated F03_Control_Mode_Manager() call site(s) to forward rtP",
            r"(&rtDW->Saturation1,\s*)(?:rtP,\s*)?(&rtDW->F03_Control_Mode_Manager_f\))",
            r"\1rtP, \2",
        ),
        (
            "updated PI_clamp_fixdtFF_c() call site(s) to forward rtP",
            r"(rtu_VqFinPrev,\s*rtb_Divide1_g,\s*rtb_Switch4_idx_1,\s*0,\s*rtb_Switch4_idx_0,\s*)(?:rtP,\s*)?(&localDW->PI_clamp_fixdtFF_ci\))",
            r"\1rtP, \2",
        ),
        (
            "updated PI_clamp_fixdtFF() call site(s) to forward rtP",
            r"(rtu_Vd_max,\s*rtu_Vd_min,\s*0,\s*rtb_Divide1_g,\s*)(?:rtP,\s*)?(&localDW->PI_clamp_fixdtFF_a\))",
            r"\1rtP, \2",
        ),
        (
            "updated FOC() call site(s) to forward rtP",
            r"(rtP->b_cruiseCtrlEna,\s*)(?:rtP,\s*)?(&rtDW->Merge\w*,\s*&rtDW->VariantMergeForOutportVq_Trq,\s*&rtDW->FOC_h\))",
            r"\1rtP, \2",
        ),
    ]

    # Heal: earlier revisions of this patcher placed the rtP argument after
    # rty_Vd (right before &rtDW->FOC_h). Remove it so the corrected insertion
    # above (after rtu_b_cruiseCtrlEna) is the only one.
    cleaned_text, n_fix = re.subn(
        r"(&rtDW->VariantMergeForOutportVq_Trq,\s*)rtP,\s*(&rtDW->FOC_h\))",
        r"\1\2",
        text,
        flags=re.MULTILINE,
    )
    if n_fix:
        text = cleaned_text
        details.append(f"removed mis-placed FOC() rtP argument(s) ({n_fix})")

    for detail, pattern, replacement in call_patterns:
        new_text, count = re.subn(pattern, replacement, text, flags=re.MULTILINE)
        if count:
            text = new_text
            details.append(f"{detail} ({count})")

    missing_rtP_calls: list[str] = []

    def _is_call_site(match_start: int) -> bool:
        prefix = text[max(0, match_start - 80):match_start]
        return re.search(r"(?:extern\s+)?void\s+$", prefix) is None

    for match in re.finditer(r"F03_Control_Mode_Manager\([\s\S]*?\);", text):
        if not _is_call_site(match.start()):
            continue
        block = match.group(0)
        if re.search(r"\brtP\s*,", block) is None:
            missing_rtP_calls.append(block)

    for match in re.finditer(r"FOC\([\s\S]*?\);", text):
        if not _is_call_site(match.start()):
            continue
        block = match.group(0)
        if re.search(r"\brtP\s*,", block) is None:
            missing_rtP_calls.append(block)

    if missing_rtP_calls:
        preview = "\n\n".join(missing_rtP_calls[:3])
        raise RuntimeError(
            "Detected call sites missing rtP forwarding. Update the patcher.\n"
            f"Examples (first 3):\n{preview}"
        )

    return text, details


def _patch_data_params_name(text: str) -> tuple[str, list[str]]:
    details: list[str] = []

    # Rename generated default param container from rtP to rtP_Left.
    # Only touch the declaration/definition in the data file.
    new_text, n = re.subn(
        r"^(\s*P\s+)rtP(\s*=)",
        r"\1rtP_Left\2",
        text,
        flags=re.MULTILINE,
        count=1,
    )
    if n:
        details.append("renamed default params: 'P rtP' -> 'P rtP_Left'")
        return new_text, details

    return text, details


def patch_file(path: Path) -> FileEdit:
    original = _read_text(path)
    text = original
    details: list[str] = []

    if path.name == "BLDC_controller.h":
        text, d = _patch_header(text)
        details += d

    elif path.name == "BLDC_controller.c":
        text, d = _ensure_local_param_ptr(text, "BLDC_controller_step")
        details += d
        text, d = _ensure_local_param_ptr(text, "BLDC_controller_initialize")
        details += d
        text, d = _patch_param_access(text)
        details += d
        text, d = _patch_generated_rtP_plumbing(text)
        details += d

    elif path.name == "BLDC_controller_data.c":
        text, d = _patch_data_params_name(text)
        details += d

    changed = text != original
    if changed:
        _write_text(path, text)

    return FileEdit(path=path, changed=changed, details=details)


def main() -> int:
    targets = [
        REPO_ROOT / "Inc" / "BLDC_controller.h",
        REPO_ROOT / "Src" / "BLDC_controller.c",
        REPO_ROOT / "Src" / "BLDC_controller_data.c",
    ]

    edits: list[FileEdit] = []
    for p in targets:
        if not p.exists():
            raise FileNotFoundError(f"Missing expected file: {p}")
        edits.append(patch_file(p))

    any_changed = any(e.changed for e in edits)

    print("Simulink post-gen patch summary:")
    for e in edits:
        rel = e.path.relative_to(REPO_ROOT)
        if e.changed:
            print(f"- patched {rel}")
            for d in e.details:
                print(f"  - {d}")
        else:
            print(f"- ok      {rel}")

    if not any_changed:
        print("No changes needed.")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
