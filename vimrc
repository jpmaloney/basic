" vimrc - John's settings.
"
" Copyright 2012 John P. Maloney
"
" Distributed under the Apache License, Version 2.0.
" (See accompanying file LICENSE_2_0 or copy at
" http://www.apache.org/licenses/LICENSE-2.0)

"
set nocompatible
"
syntax on
"
if has( "gui_running" )
  " Open Vim maximized.
"  :au GUIEnter * vsplit
"  :au GUIEnter * vertical resize 90 
"  :au GUIEnter * simalt ~x
  
  " Use nice programming font.
"  set guifont=Bitstream\ Vera\ Sans\ Mono:h10 
"  set guifont=consolas:h10
"  set guifont=Monospace:h10
endif

" Tab handling.  Ref: http://www.jwz.org/doc/tabs-vs-spaces.html
set softtabstop=4
set shiftwidth=4
set tabstop=4
"set smarttab sta
set expandtab

set lines=50
"what's the difference between columns and textwidth?
set columns=80
"set textwidth=80

" allow <BkSpc> to delete line breaks, beyond the start of the current
" insertion, and over indentations:
set backspace=eol,start,indent

set cindent

set visualbell

" Highlight matching text.
set hlsearch

" Jump to matching text as you type.
set incsearch

" Case-insensitive search...
set ignorecase
" ...unless search string has uppercase characters.
set smartcase

" FileOpen dialog uses current directory.
set browsedir=buffer

" Plugin: minibufexpl.vim from www.vim.org.
" Navigate windows using control + arrow keys.
let g:miniBufExplMapWindowNavVim = 1
let g:miniBufExplMapWindowNavArrows = 1
let g:miniBufExplMapCTabSwitchBuffs = 1
let g:miniBufExplModSelTarget = 1

" Use Boost.Jam as make program.
set makeprg=bjam

map <C-J> <C-W>j<C-W>_
map <C-K> <C-W>k<C-W>_

" Split window (vertical).
map <F2> <C-W>v
" Split window.
map <F3> <C-W>s

" Close window.
map <F4> <C-W>c

" Set printer font.
set pfn=Bitstream\ Vera\ Sans\ Mono:h8 

set ruler
set showmode
set showcmd

set equalalways
set splitright
set splitbelow

" Create side-by-side windows.
" vsplit

":au GUIEnter * <C-W-=>

"Clear the colors for any items that you don't like
hi clear StatusLine
hi clear StatusLineNC 

"Set up your new & improved colors
hi ErrorMsg     guifg=white     guibg=Red         gui=bold
hi ModeMsg      guifg=black     guibg=LightGreen  gui=bold
hi StatusLine   guifg=LightCyan guibg=blue        gui=bold 
hi StatusLineNC guifg=white     guibg=gray
hi VertSplit    guifg=gray      guibg=white
hi WarningMsg   guifg=white     guibg=DarkMagenta gui=bold

"tags:
"set tags=dir1/tags,dir2/tags

