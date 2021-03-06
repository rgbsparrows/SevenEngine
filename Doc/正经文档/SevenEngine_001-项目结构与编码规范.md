SevenEngine的开发其实已经进行了一段时间了，这里对之前完成的工作进行一些简单的描述（由于受UE的影响比较深（除了公司的引擎之外，UE差不多是我唯一一个读过源码的引擎），所以很多地方会与UE有一些相近）
# 项目的目录结构
SevenEngine的目录结构接近于UE4的目录结构(早期的时候尝试使用魔改过的ubt作为项目生成工具，所以比较接近)，但存在一些差异，在此进行相对详细的描述

* Binaries
    * Debug
    * Shipping
    * Test
    * Profile
    * note: Binaries文件夹下存在Debug, Shipping, Test, Profile四个文件夹，分别对应于相应的编译配置。
* CMakeFile
    * SdkList.txt
        * 用于配置所依赖的sdk，其主要的配置内容包括Sdk的debuglibrary，releaselibrary，以及include路径
    * CMakeLists.txt
        * 实际用于生成项目的CMakeLists.txt文件，一般在项目生成或项目结构没有什么变更时不需要改变，所以内容在大多数情况下也可以忽略
* Config
    * 储存配置文件
* Intermediate
    * 储存通过cmake所生成的project文件
* Shaders
    * 储存shader文件
* Source
    * 储存代码
* GerneateProjectFile.bat
    * 用于生成vs项目文件的bat，其中会调用cmake命令行，如果在后面追加-force参数，那么会删除Intermediate文件夹，而后重新生成
* OpenProject.bat
    * 打开sln文件，如果不存在相应的项目文件，那么会尝试生成项目文件

虽然这部分是引擎的项目文件，但项目也会遵循基本一致的文件结构，另外暂时cmake没有扩展项目相关的内容，所以不多赘述

# 项目中的代码组织
代码一般以模块的方式组织，模块表示一组相对内聚的功能集合，每个模块由其相应的模块接口类管理，一般来说，一个模块只能通过模块接口类（包括由接口类所创建出的对象）被访问，这样可以限制一个模块的所有对外接口都被限制在以模块接口类为根节点的树中，增强可维护性

但是有这些模块时不在上述范围要求中的，这些模块的功能往往难以通过接口进行描述，其并不是单纯的一组功能的集合，这些模块主要分为三部分
* 作为引擎的核心组件的内容，这部分内容是超出模块存在的，现在来说，主要有如下两部分，分别表示引擎的一些基础核心组件和主函数
    * Core 
        * Core部分所提供的功能包括项目路径，项目配置，命令行解析等，同时还包括一些宏定义之类的功能，难以，也不方便通过接口的方式进行表示
    * EngineMain
        * EngineMain作为引擎的主函数部分，所以并不会提供功能以让外部调用，也就不需要存在相应的接口
* 用作对其他的api的简单封装，这部分内容逻辑上也可以用模块接口进行封装，但这些内容只是对其他的一些api的简单封装，如果使用模块接口的方式进行封装，一方面显得冗余，另一方面也会干扰运行效率
    * CRTLibrary
    * WindowsPlatform
* (暂时还没有)作为一些用户定义类的集合的模块，这部分模块更大的意义是作为类的集合，而类本身则具备基础的反射功能，这使得引擎可以越过模块，直接访问模块内部的类，也就使得模块接口失去了相应的意义，不过在有需要的情况下，也可以二合一，一方面在模块内部定义用户定义类，另一方面也提供一些模块功能

# 现在项目中的一些代码规范
* 具体一些例如命名规范等比较细碎的内容在这里不多赘述，可以参考[SevenEngine的wiki](https://github.com/rgbsparrows/SevenEngine/wiki/%E7%BC%96%E7%A0%81%E8%A7%84%E8%8C%83)
* 鼓励使用cpp17,20的内容
* 尽可能避免直接使用各种api，各种api的宏，类型等应该全部隐藏在模块内部，以避免对外部的其他代码造成影响，否则以win32 api为例，其中会有大量的极易与其他函数重名的宏（这也是要有WindowsPlatform等模块的主要原因，它以一种简单粗暴的方式完成了对这些内容的封装）
* 尽可能不要使用单例，除了完全独立的，不从属于其他对象的管理类以及EngineLoop外，应该不存在其他的单例类
* 避免毫无节制的滥用模板，以引发编译时间，生成文件大小的过度膨胀（不过模板本身还是个很香的东西的）
* 所有的函数都应该是noexcept的，以谋取到更大的编译器优化空间
* 鼓励使用sal注释来进行代码约束和检查
* 不要乱用宏，宏的作用应该体现在拼接和生成代码片段上，并且应该优先考虑是否可以通过模板来替代宏
* 引擎中应该全部使用宽字符串，且更多的利用string_view来进行传参，以提供更大的优化空间，避免不想要的对象拷贝(虽然对于一些函数，例如win32 api中的部分函数来说，其需要以空字符串结尾的字符串，这时依旧需要去构造新的字符串，但对于引擎总体而言，更统一，消耗更低的传参方式有机会在未来争取到更大的优化空间)
* 对于存在中文等字符的文件，文件的编码方式必须被指定为utf8-bom，以避免在编码方式上引发异常，导致出现乱码等情况
* 坚决避免重复代码/重复对象（例如字符串）的出现，除非他们看起来一样，但是的确有着不同的含义