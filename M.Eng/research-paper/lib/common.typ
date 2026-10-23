// 各论文共用的配色与排版函数
#let code-bg = rgb("#f6f8fa")
#let code-border = rgb("#d0d7de")
#let code-fg = rgb("#1f2328")

// 伪代码排版：行号 | 缩进内容 | 右侧注释
#let kw(s) = text(weight: "bold", s)
#let algorithm(title, input, output, lines) = {
  let rows = ()
  for (i, l) in lines.enumerate() {
    let (ind, body, ..rest) = l
    rows.push(text(fill: luma(130))[#(i + 1)])
    rows.push(pad(left: ind * 1.4em, body))
    rows.push(if rest.len() > 0 { text( fill: luma(110))[\/\/ #rest.at(0)] })
  }
  block(
    width: 100%,
    breakable: false,
    stroke: (top: 1pt, bottom: 1pt),
    inset: (x: 2pt, y: 7pt),
  )[
    #set par(justify: false)
    算法#h(0.6em)#title
    #v(-0.3em)
    #line(length: 100%, stroke: 0.5pt)
    #v(-0.3em)
    输入：#input \
    输出：#output
    #v(-0.3em)
    #line(length: 100%, stroke: 0.3pt + luma(160))
    #v(-0.2em)
    #grid(
      columns: (1.4em, 1fr, auto),
      column-gutter: 0.6em,
      row-gutter: 0.7em,
      align: (right + horizon, left + horizon, right + horizon),
      ..rows,
    )
  ]
}

