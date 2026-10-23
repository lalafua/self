# 登录交互式 shell 时自动进入 tmux（附加到已有的 main 会话，没有则新建）
if status is-interactive
    and not set -q TMUX
    and not set -q TMUX_DISABLE # 临时禁用：TMUX_DISABLE=1 fish
    and not set -q INSIDE_EMACS
    and test "$TERM_PROGRAM" != vscode
    and command -q tmux
    exec tmux new-session -A -s main
end
