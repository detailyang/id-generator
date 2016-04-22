recommended production deploy as follow:
![production](https://rawgit.com/detailyang/id-generator/master/docs/deploy.jpg)

Also, you can use any TCP load balancer to instead of LVS, it's up to what you like:)
As we see, we set three id-generatord behind lvs. And every id-generatord has different offset value and the same step value. Like as follow:

````bash
A: 1 4 7 10 13
B: 2 5 8 11 14
C: 3 6 9 12 15
````

The arch's disadvantage is the id by *step* command will be not linear growth and just Like `1 4 7 2 5`.
