#!/usr/bin/env python3
"""Generate Appendix Б — test-case inputs and program outputs as a PDF."""

import sys
import subprocess
from pathlib import Path
from html import escape

from pygments import highlight
from pygments.lexers import get_lexer_by_name
from pygments.formatters import HtmlFormatter
from weasyprint import HTML

examples = sorted(Path("examples").glob("*.hex"))

formatter = HtmlFormatter(style="friendly", linenos=True)
css = formatter.get_style_defs(".highlight")
text_lexer = get_lexer_by_name("text")

sections = []
for example in examples:
    name = example.stem.replace("-", " ").replace("_", " ").title()
    source = Path(example).read_text()
    highlighted_source = highlight(source, text_lexer, formatter)

    result = subprocess.run(
        ["./build/apps/app", "-i", str(example), "-x"],
        capture_output=True,
        text=True,
        encoding="utf-8",
        errors="replace",
    )
    output = result.stdout + result.stderr

    sections.append(
        f'<h2>{name}</h2>\n'
        f'<h3>{example}</h3>\n'
        f'{highlighted_source}\n'
        f'<h3>Output</h3>\n'
        f'<pre class="output">{escape(output)}</pre>\n'
        f'<div style="page-break-after: always;"></div>'
    )

html = (
    "<!DOCTYPE html>\n<html><head><style>\n"
    f"{css}\n"
    "@page { margin: 1.5cm; }\n"
    "body { font-family: monospace; font-size: 12px; margin: 0; }\n"
    ".highlight { background: none !important; }\n"
    ".highlight pre { background: none !important; }\n"
    "h1.title { font-family: 'Times New Roman', Times, serif;"
    " font-size: 18px; font-weight: bold; text-align: center; }\n"
    "h2 { font-family: 'Times New Roman', Times, serif;"
    " font-size: 18px; font-weight: bold; color: #333;"
    " border-bottom: 1px solid #ccc; padding-bottom: 0.3em; }\n"
    "h3 { font-family: 'Times New Roman', Times, serif;"
    " font-size: 14px; font-weight: bold; color: #555;"
    " margin-top: 1em; }\n"
    "pre.output { font-family: monospace; font-size: 10px;"
    " padding: 0.5em; border: 1px solid #ccc; white-space: pre; }\n"
    "</style></head><body>\n"
    '<h1 class="title">Додаток Б. Результати тестування</h1>\n'
    f"{''.join(sections)}\n"
    "</body></html>"
)

output_dir = Path("docs")
output_dir.mkdir(parents=True, exist_ok=True)
output = (
    sys.argv[1]
    if len(sys.argv) > 1
    else str(output_dir / "lab_3_kv33_Kozlov_addon_B.pdf")
)
HTML(string=html).write_pdf(output)
print(f"Generated {output}")
