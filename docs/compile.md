#Compile

look at
````bash
git clone https://github.com/detailyang/id-generator.git
cd id-generator
make distclean
(
    cd deps
    make hiredis jemalloc linenoise lua
)
make PREFIX=/data/xxx install
````