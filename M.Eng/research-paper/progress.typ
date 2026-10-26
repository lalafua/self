#set page(
	paper: "a4",
	margin: (x: 2.2cm, y: 2cm),
	numbering: "1",
)

#set text(
	font: ("Source Han Serif", "Source Han Sans"),
	size: 11pt,
	lang: "zh",
)

// 代码中的中文回退到简体等宽字体
#show raw: set text(
	font: ("Source Code Pro", "Source Han Mono SC"),
	size: 11pt
)

// 公式中的中文使用正文宋体，避免回退到其他字体
#show math.equation: set text(font: ("New Computer Modern Math", "Source Han Serif"))

// 表格字号略小于正文，避免多列表格换行
#show table: set text(size: 10pt)

#set list(marker: ([•], [–]))

// GitHub light 配色
#import "lib/common.typ": code-bg, code-border, code-fg

// 行内代码：小圆角底色块
#show raw.where(block: false): it => box(
  fill: rgb("#afb8c133"),
  inset: (x: 3pt),
  outset: (y: 2pt),
  radius: 3pt,
  it,
)

#show raw.where(block: true): it => block(
  width: 100%,
  inset: 10pt,
  radius: 6pt,
  fill: code-bg,
  stroke: 0.5pt + code-border,
)[
  #set text(fill: code-fg)
  // 右上角显示语言名，没写语言的代码块不显示
  #if it.lang != none {
    place(top + right, dx: 4pt, dy: -6pt, text(size: 11pt, it.lang))
  }
  #it
]

#set par(
	justify: true,
	leading: 0.8em,     // 中文行距
	spacing: 1.6em,     // 段间距
	// first-line-indent: (amount: 2em, all: true),  // 段首缩进 2 字符
)

// 只给带标签（会被 @ 引用）的行间公式编号
#set math.equation(numbering: "(1)")
#show math.equation.where(block: true): it => {
	if it.has("label") or it.numbering == none { it } else {
		math.equation(block: true, numbering: none, it.body)
	}
}

// 标题与上下文的间距：above 是与上文的距离，below 是与正文的距离
// em 相对于标题自身字号，一级、二级标题字号更大，实际间距也更大
#show heading: set block(above: 2.2em, below: 1.2em)

// 一级、二级标题居中
#show heading.where(level: 1): set align(center)
#show heading.where(level: 2): set align(center)

#align(center)[
	#text(size: 22pt, weight: "bold")[论文阅读报告]
	#v(0.5em)
]


#outline(title: "目录", depth: 2)

#pagebreak()

#include "papers/SRFM.typ"

#include "papers/AGC-VLN.typ"

#include "papers/VLNontheFly.typ"

#include "papers/ASENA.typ"

#include "papers/ViSA-VLN.typ"
