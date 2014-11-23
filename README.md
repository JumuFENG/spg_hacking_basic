spg_hacking_basic
=================
搜集一些Windows平台单机游戏修改的基本技术，首先会加入一些诸如动态内存修改，Windows消息钩子、Windows API Hook之类的内容。
第一步准备实现内存动态修改，然后准备实现变速齿轮类似的功能
本人也是边学边写，欢迎志同道合者加入或为我提供相关学习资源。

some basic technoligy for hacking single_player games on Windows.

Firstly, will add some content like memory dynamicly modifying, Windows Message Hook, Windows API Hook, and so on.
I'm don't know all the techs right now, welcome to join me and give me advices.

*cczAssist*
《曹操传》内存修改器，用JUCE开发的界面。处理界面输入输出及基本的转换。

*libAssist*
《曹操传》内存修改器主功能模块，目前包括自动点击、修改内存值、变速等功能

*libIPCO*
跨进程操作模块，主要用于注入到《曹操传》进程中进行键盘捕获等操作

*libMemDO*
动态内存操作模块，该模块提供内存操作的一些功能

*pvztest*
用于测试libMemDO模块， 目前功能：修改植物大战僵尸 中某内存值，使得种植植物不消耗阳光值
