#!/usr/bin/env python3
"""Generate Appendix A — full source listing as a PDF."""

import sys
from pathlib import Path
from string import Template

from pygments import highlight
from pygments.lexers import CppLexer
from pygments.formatters import HtmlFormatter
from weasyprint import HTML

files = [Path("apps/app.cpp")]
modules = sorted(set(p.parent for p in Path("src").rglob("*.hpp")))
for module in modules:
    files.extend(sorted(module.glob("*.hpp")))
    files.extend(sorted(module.glob("*.cpp")))

integration = sorted(Path("tests/integration").rglob("*.cpp"))
files.extend(integration)

formatter = HtmlFormatter(style="friendly", linenos=True)
css = formatter.get_style_defs(".highlight")
lexer = CppLexer()

sections = []
for f in files:
    code = Path(f).read_text(encoding="utf-8")
    highlighted = highlight(code, lexer, formatter)
    sections.append(
        f'<h2>{f}</h2>\n{highlighted}\n'
        f'<div style="page-break-after: always;"></div>'
    )

template = Template((Path(__file__).parent / "sources.html").read_text(encoding="utf-8"))
html = template.substitute(pygments_css=css, content="".join(sections))

output_dir = Path("docs")
output_dir.mkdir(parents=True, exist_ok=True)
output = (
    sys.argv[1]
    if len(sys.argv) > 1
    else str(output_dir / "lab_3_kv33_Kozlov_addon_A.pdf")
)
HTML(string=html).write_pdf(output)
print(f"Generated {output}")
