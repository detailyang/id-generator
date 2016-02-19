#Configure

configure file is the same as the redis config file, and i add some custom command like
````bash
step_offset 1|2|3...
step_begin_keyxxx 10000|500...
datacenter_id 1|2|3...
````
there is a tips that we 'must' enable aof and let appendfsync always, so step command will not lost data.Else if host down, we maybe lost data because of disk cache :angry:
````bash
##################
#use `always` to persistence data, in case of step command lost data
##################
appendfsync always
no-appendfsync-on-rewrite no
auto-aof-rewrite-min-size 64mb
auto-aof-rewrite-percentage 100
aof-rewrite-incremental-fsync yes
appendfilename id0.aof
