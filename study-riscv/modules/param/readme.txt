$ sudo insmod param_test.ko value=4 name="str" arr=1,2,3,4

//실행 로그
[  139.792166] ===== Print Params =====
[  139.795812] value = 4 
[  139.798224] cb_value = 0 
[  139.800886] name = str
[  139.803287] arr[0] = 1 
[  139.805772] arr[1] = 2 
[  139.808267] arr[2] = 3 
[  139.810756] arr[3] = 4 


$ cd /sys/module/param_test/parameters
$ sudo sh -c "echo 77 > cb_value"

//실행 로그
[  263.081696] Call back function called...
[  263.085833] New value of cb_value = 77

