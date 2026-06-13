#!/usr/bin/env python3
"""Generate Appendix Б — test-case inputs and program outputs as a PDF."""

import sys
import subprocess
from pathlib import Path
from html import escape
from string import Template

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
        f'<h3>{example}</h3>\n'
        f'{highlighted_source}\n'
        f'<h3>Output</h3>\n'
        f'<pre class="output">{escape(output)}</pre>\n'
        f'<div style="page-break-after: always;"></div>'
    )

template = Template((Path(__file__).parent / "tests.html").read_text(encoding="utf-8"))
html = template.substitute(pygments_css=css, content="".join(sections))

output_dir = Path("docs")
output_dir.mkdir(parents=True, exist_ok=True)
output = (
    sys.argv[1]
    if len(sys.argv) > 1
    else str(output_dir / "lab_3_kv33_Kozlov_addon_B.pdf")
)
HTML(string=html, base_url=str(Path.cwd())).write_pdf(output)
print(f"Generated {output}")
