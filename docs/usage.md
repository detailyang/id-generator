#Usage
i delete unsed redis commands and add these commands
##step
````
192.168.33.10:6379> step key
(integer) 2
192.168.33.10:6379> step key
(integer) 3
192.168.33.10:6379>
````
step is the auto incrment integer like mysql and we can set the step and the starting value

##snowflake
````
(integer) 6106511272512196609
192.168.33.10:6379> snowflake
(integer) 6106511275431432194
192.168.33.10:6379> snowflake
(integer) 6106511277620858883
````
snowflake is the twitter snowflake

##objectid
````
192.168.33.10:6379> objectid
"56C75C0B00000113DB000001"
192.168.33.10:6379> objectid
"56C75C0B00000113DB000002"
````
objectid is the mongodb objectid

##serialid
````
192.168.33.10:6379> serialid
(integer) 160219181722229378
192.168.33.10:6379> serialid
(integer) 160219181722229379
````
serialid is human readable, 160219181722229379 means 2016:02:19 18:17:22 + random number

##hget
It is no longer hash. It's hacked
````
192.168.33.10:6379> hget step key
(integer) 6
192.168.33.10:6379> hget snowflake anything
(integer) 6106513511146459141
192.168.33.10:6379> hget objectid anything
"56C75DE800000113DB000003"
````
hack hget, make hget command key == command key, because some redis client dont support custom command :alien:

other commands include `info`, `select`, `del`, `set`, `keys`, `bgrewriteaof`, `ping`.
