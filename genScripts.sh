以flex为例，
先要求配置环境，将javamts下的两个Jar包antlr-2.7.6.jar和mts.jar添加到java路径的lib里。
export CLASSPATH=$CLASSPATH:/home/ccsu/ObjectTools/mts/lib/antlr-2.7.6.jar:/home/ccsu/ObjectTools/mts/bin/mts.jar

对V2版本来说，有两个测试套件：
java sir.mts.MakeTestScript -sf ../testplans.alt/v2/v0.tsl.universe -sn ../scripts/runall0.sh -en flex.int.exe -tg bsh
java sir.mts.MakeTestScript -sf ../testplans.alt/v2/v2.tsl.universe -sn ../scripts/runall2.sh -en flex.int.exe -tg bsh

对V4版本来说，有两个测试套件：
java sir.mts.MakeTestScript -sf ../testplans.alt/v4/v0.tsl.universe -sn ../scripts/runall0.sh -en flex.int.exe -tg bsh
java sir.mts.MakeTestScript -sf ../testplans.alt/v4/v2.tsl.universe -sn ../scripts/runall2.sh -en flex.int.exe -tg bsh

对grep来说，
   针对v0_1.tsl.universe，加参数-nesc，不使用转义
   v0_2.tsl.universe，不加参数-nesc
java sir.mts.MakeTestScript -sf ../testplans.alt/v1/v0_1.tsl.universe -sn ../scripts/runall0.sh -en grep.int.exe -nesc -tg bsh
java sir.mts.MakeTestScript -sf ../testplans.alt/v1/v0_2.tsl.universe -sn ../scripts/runall2.sh -en grep.int.exe -tg bsh
