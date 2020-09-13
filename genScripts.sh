以flex为例，

对V2版本来说，有两个测试套件：
java sir.mts.MakeTestScript -sf ../testplans.alt/v2/vo.tsl.universe -sn ../scripts/runall0.sh -en flex.int.exe -tg bsh
java sir.mts.MakeTestScript -sf ../testplans.alt/v2/v2.tsl.universe -sn ../scripts/runall2.sh -en flex.int.exe -tg bsh

对V4版本来说，有两个测试套件：
java sir.mts.MakeTestScript -sf ../testplans.alt/v4/vo.tsl.universe -sn ../scripts/runall0.sh -en flex.int.exe -tg bsh
java sir.mts.MakeTestScript -sf ../testplans.alt/v4/v2.tsl.universe -sn ../scripts/runall2.sh -en flex.int.exe -tg bsh
