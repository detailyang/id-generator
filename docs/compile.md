#Compile

look at
````bash
git clone https://github.com/detailyang/id-generator.git
cd id-generator
# not resolve jemlloc:(, use glibc replace
make PREFIX=/opt/id-generator MALLOC=libc install
````
