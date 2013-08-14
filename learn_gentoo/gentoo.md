Gentoo learning notes
==========

### 准备工作 ###

- 准备环境

> 我本机电脑是Intel Core i7 四核八线程,固态硬盘,给虚拟机分配的是8G硬盘,256M内存,两个核给它

>> 后来发现硬盘8G太少,装完KDE图形界面之后就只剩下2G不到了,内存也太小,只用Terminal时还行,一旦启动KDE后,速度非常卡,后来只能给它1G的内存

- 了解了Live CD/stage3/minimized CD的区别,下载镜像文件等

> **LiveCD**包含了所有需要的文件,包括系统,各种软件等等,直接用它的话就等于失去了自己编译安装的意义了

> **stage3**是一个含有最小Gentoo环境的压缩包,有了它就可以自行编译安装系统了

> **minimized CD**是用来安装的gentoo的引导ISO,用它可以从只有硬盘,无操作系统的计算机里启动gentoo的安装

- 了解了linux下如何分区,如何创建文件系统

> 使用**fdisk**来进行分区

	fdisk /dev/sda

	Command (m for help): h

> 通过官方Guide来创建文件系统


- 了解了一些和安装linux系统相关的知识,如chroot,mount一些路径等

> **chroot**是在unix系统的一个操作,用于对当前的程序和它的子进程改变真实的磁盘根目录,它往往被用在启动引导光盘并安装系统时

### 编译内核 ###

- USE用于编译选项

> **USE**是Gentoo为用户提供的最具威力的变量之一。很多程序通过它可以选择编译或者不编译某些可选的支持。例如，一些程序可以在编译时加入对gtk或是对qt的支持

- 两种编译内核方式(耗时50分钟)

 * 手动配置编译内核
 
> 手动配置内核其实是最困难的步骤,因为这需要对系统有一定深入的了解,特别是如果在虚拟机Vmware中安装gentoo的话,需要配置各种专为虚拟机环境所设置的参数,在网上很多都BLOCK在这个步骤上,因为一旦参数忘记配置,那么编译后的内核是无法启动的

	make menuconfig

> 确保启动系统所必需的驱动,如SCSI硬盘驱动,VMware网卡驱动等等

	-->Device Drivers-->SCSI device support--><*>SCSI disk support
	-->Device Drivers-->SCSI device support-->SCSI low-level drivers-->
	     <*>BusLogic SCI support-->FlashPoint support
	-->Device Drivers-->Fusion MPT device support-->all
	-->Device Drivers-->USB support--><*>USB Mass Storage support
	-->Device Drivers-->Networking support-->Ethernet driver support(NEW)-->
	     <*>AMD PCnet32 PCI support
	 -->Processor type and features-->Processor family-->Pentium M
 
 * 使用genkernel

> 如果使用**genkernel**编译的话,那就非常方便了,这样会把所有硬件的驱动全部都编译到gentoo的内核中,除非你的硬件比较特殊,否则一般情况下,这样的内核不会出现硬件驱动不兼容的问题

>执行命令(耗时50分钟左右):

	#genkernel all

- 文件系统信息

 - fstab

> 在Linux系统下,系统所用到的所有分区都必须在/etc/fstab文件中指明.这个文件包含了这些分区的挂载点（在系统目录树中的位置）,挂载方法和特殊挂载选项（是否自动挂载,是否可以用户挂载等）

> 简而言之,我觉得它就是硬盘分区表,有了它,内核才知道怎么把物理硬盘和系统中的逻辑硬盘对应起来

> 要注意的是IDE接口的硬盘和SCIS的硬盘有细微不同,一个是sda,一个是hda,具体可查阅网上资料,无数网友BLOCK在此处

### 初始化系统 ###

- 重新启动系统,进入自己编译的内核

- **问题:忘记设置root密码**

> 迫不及待地想体验gentoo的后果就是root密码忘记设置了,这点和windows有所不同,windows如果不设置密码,那密码就是空,但是Linux的话,不设置密码,那你不管怎么输入密码都是错误的,于是乎,只能重新启动光盘,重新设置

 - 重新用CD进去
	
 - 新建一个shell改密码


- **问题:无法上网**

> 这个东西谷歌百度必应stackoverflow上都没找到答案,后来只能自己乱试,最后得出结论需要自己添加

	/etc/udev/rules.d/70-persistent-net.rules

	SUBSYSTEM=="net", ACTION=="add", DRIVERS=="?*", ATTR{address}=="00:e0:4c:90:06:83", ATTR{type}=="1", KERNEL=="eth*", NAME="eth0"

 - 首先ifconfig -a
	
 - 配置该特殊网卡enp2s1(我不知道怎么改名字)
	
 - ifconfig enp2s1 up
	
 - /etc/init.d/net.enp2s1和net.lo 软连接


- 尝试安装vim成功 :-)

> gentoo的自带编辑器竟然vi都没有,只有nano这个以前没用过的editor,让我这个vim控情何以堪,于是果断在系统启动后先装个vim试试

	emerge vim

> 装完后看了下,是最新的vim7.3,没什么问题

- rc-update add sshd default 添加sshd为自启动
	
> 在vmware里面连复制粘贴都不能,所以需要尽快启动sshd让putty能连上去


### 安装图形界面KDE ###

- eselect profile list指定desktop(KDE)

- emerge kdebase-meta(编译了8小时)

- 安装X11服务器

- **问题:KDE启动后鼠标键盘失灵**

 - emerge xf86-input-evdev

路径:

	/etc/conf.d/hostname
	
	/etc/conf.d/net
	
	/etc/make.conf
	
	/etc/portage/make.conf
