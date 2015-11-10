"Pathogen Package Manager
execute pathogen#infect()
syntax on
filetype plugin indent on

"Search Features
set incsearch
set ignorecase
set smartcase
set hlsearch
nmap \q :nohlsearch

"Switching Buffers and Tabs
set splitbelow
set splitright
nnoremap <C-J> <C-W><C-J>
nnoremap <C-K> <C-W><C-K>
nnoremap <C-L> <C-W><C-L>
nnoremap <C-H> <C-W><C-H>

"Tab Pages
nnoremap <C-Left> :tabprevious<CR>
nnoremap <C-Right> :tabnext<CR>
nnoremap <silent> <A-Left> :execute 'silent! tabmove ' . (tabpagenr()-2)<CR>
nnoremap <silent> <A-Right> :execute 'silent! tabmove ' . tabpagenr()<CR>

"Tabs
set expandtab shiftwidth=4 softtabstop=4
set autoindent

"Showing Hidden Characters
set listchars=eol:$,tab:>-,trail:~,extends:>,precedes:<

if list
    noremap <C-a> :set list<CR>
else
    noremap <C-a> :set list&<CR>
endif

"CUA hotkeys
nnoremap <C-s> :w<CR> 
nnoremap <C-c> <y>
nnoremap <C-q> :wq
