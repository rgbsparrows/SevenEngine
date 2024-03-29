* 下一步需要构建的是主要是渲染线程，但在编写渲染线程的过程中，需要考虑渲染线程与引擎中其他组件的交互手段
* 术语表
    * FrameResource
        * 渲染过程中，每一帧的渲染工作所依赖的资源
    * CommonRenderResources
        * 整个渲染过程中，可以被每一帧共享的资源，不独属于某个FrameResource，例如Texture
    * 逻辑层
        * UI与世界游戏逻辑等
* 渲染线程的整个编写过程中，暂时想到的有下面这几个点都需要进行
    * 渲染线程的执行过程
        * 在执行过程中，遵循这样的逻辑
            * BeginFrame
                * 等待逻辑线程将渲染所需要的数据完整的传输/更新到逻辑线程
                * 等待上一次使用了当前帧缓冲数据的所有GPU任务执行结束，依赖Fence做一次Yield即可
            * CustomEvent
                * 执行由逻辑线程向渲染线程所传输的命令
            * FrameTick
                * 更新各种状态
                * 执行渲染过程，上传资源等常规的渲染操作                    * 提交/更新后续渲染所依赖的GPU资源，例如CB，同时在这一阶段可以选择去释放上一次的对应帧中的相应资源，亦或者是创建新的资源
            * EndFrame
                * 执行一些后期的运行操作，例如窗口的Present等
                * 提交新的GPUFence
                * 通知逻辑线程，当前FrameResource重新变回可用的状态
    * 在这个过程中的注意事项
        * 在FrameTick执行的前后的环境是干净的，不需要进行额外的清理工作
            * 即我再FrameTick中的任何操作，都不应该使得后续的FrameTick需要做额外的工作，才能保证FrameTick的正确执行
        * RenderGraph的执行过程被完全局限在FrameTick中
        * GUI的绘制不属于RenderGraph的一部分，将GUI的绘制直接强耦合在渲染线程中，而不是额外构建一个RenderGraph
            * 既然GUI的绘制不属于RenderGraph的一部分，而是由渲染线程直接管理，那么的话Present之类的处理也可以同样由渲染县城直接进行管理了
            * 这样的话RenderGraph所执行的任务就可以局限到只在一张RT或者是多张RT上进行绘制，其余的操作都不需要考虑

* 这样的话整个渲染线程好写，其中的困难点和卡点主要还是出在各种渲染对象的创建及信息传递交互上，这部分同时在和UI，GamePlay发生交互，并且存在着可能较为复杂多变的情况，例如我之前一直心念的一个Word用两个RenderGraph同时渲染，并且用能左右横拉的两个框来表示窗口的范围
* 如果按照现在GUI完全由渲染线程来控制的方法的话，之前所设想的两个窗口使用不同的RDI来渲染的机制就泡汤了
    * 这个机制重要么，是应该用全局唯一的RDI还是说把RDIFactory局限到全局唯一，如果局限到全局唯一的话RDI那里需要做一些简单的修改
    * RDI这里最大的目标其实还是在那个3d建模上，只有那个是打算去搞一波RDI的部分
    * 感觉还是移除多RDIFactory的机制，只保留其中的一份，这样的话逻辑上应该能简单一些
* 如果RDI转成了全局唯一，那么的话有一些对象的创建或许就有了更大的程序上的自由，例如SwapChain与贴图
    * 如果使用多RDI的机制的话，SwapChain，Texture等对象是无法在渲染线程，或者说RenderGraph以外的地方创建的，因为逻辑层并不知道，当前在用什么来做渲染
* 最终定下的话，RDI资源的创建还是放在渲染线程的主线程来执行，但资源本身由逻辑线程的对象持有，创建事件通过向渲染线程发送命令的机制来进行
* 用于辅助同步操作的Event和Fence是否应该属于FrameResource的一部分，他们标示的实际上应该是当前FrameResource的状态，但对于渲染线程和逻辑线程来说，其实他们都不关心FrameResource是什么状态，他们只关心是否能拿到FrameResource
* 如果从这个角度来讲的话，那么我倾向于将Event也由渲染线程来直接管理
* 这样的话在同步过程中，相关的对象就有这么几个
    * 在BeginFrame_GameThread时阻塞，在EndFrame_RenderThread时重置的event
    * 在BeginFrame_RenderThread时阻塞，在EndFrame_GameThread时重置的event
    * 在BeginFrame_RenderThread时阻塞，在EndFrame_RenderThread时设置的GpuFence
* 我们可以把上面这几个对象放在一起，作为一个Sync对象，或者是干脆就还是单放，单放的话或许能省点事
* 梳理一下开发过程，顺便也深入理解一下重构式开发
    * 第一步的话先完成渲染线程的异步同步操作
        * 可以在逻辑线程，渲染线程正确的完成同步操作，保证对FrameResource的正常操作
        * 在逻辑线程和渲染线程可以正常的访问到正确的FrameResource
        * 能够做到刷新管线的操作
    * 在脱离世界，资源等的情况下，进行一些初步的渲染操作，保证RDI的正确执行
    * 扩充FrameResource，增加IMGUI相关的数据填充，并完成相应的渲染
    * 进行RenderGraph相关的设计，这部分内容的具体事宜放在后续考虑，暂时先以IMGUI的达成作为一个阶段性目标
* 同步过程中的特殊操作
    * 同步过程中较为特殊的点主要有两个
    * 一个是阻塞，直到当前传入的所有GPU任务都执行完毕，对应的逻辑应该是当前逻辑线程所对应的FrameResource在渲染线程，GPU都完全执行完毕
        * 这个里面有一个问题，当前的FrameResource只会在当前的逻辑帧结束后，才会进入渲染阶段，以及相应的执行，所以说如果想要做FrameResource同步，所以或许只能做到等待之前所有帧的任务执行完毕，但似乎主要应用的情况都是去取出GPU的运算结果时，才需要进行这样的操作
        * 那么的话这个情况可能是类似于什么样，可能是说我传入一个Shader来对图片进行修改，然后我又把结果取出来，存到磁盘上，如果是这样的话，我们得怎么弄
            * 如果按照上面的设计的话，我们相当于最多只能等待到上一帧运行结束，没办法将当前帧进行额外的裁断，那么如果我们要做到刚才的操作，应该得怎么做？
            * 感觉上似乎没太多好的办法，整个来看比较gg，那这样的话相当于我们需要放弃掉从GPU资源中回读到内存的功能，不过似乎倒也不是什么特别大的问题，不是不能接受
        * 如果是这样的话，那么这个阻塞还有什么用处，是否需要保留阻塞这个功能
        * 重新思考了下，回读是一个必要的功能，除非完全放弃SevenEngine中进行资源烘焙的功能，不过考虑到难以通过阻塞的方式来进行，那么的话我们可能就需要让这部分内容用类似于异步轮训之类的方式来进行了，代码一定程度上会变得更复杂，但或许可以接受？也可能接受不了，我不太说得准，那大致的逻辑会是什么样，逻辑线程在当前的FrameResource中部署任务，任务的执行结束的标志呢，任务执行结束应该是需要等待到当前的FrameResource被渲染县城第二次重用时，才会开始标识GPU任务的执行结束
        * 整个过程中可能的回调都会有哪些
            * 回调1 逻辑线程会请求去在渲染线程创建资源
            * 回调2 逻辑线程会请求某些任务完全执行完毕后的Notify
        * 这样的话，整个过程中，回调1是比较简单的，直接传数据即可，回调2复杂一些，因为其中存在一个情况，就是是当FrameResource第二次被渲染线程使用时，才能明确标识相应的任务执行完毕，那么的话相当于一个FrameResource中，需要维持两份回调2数据，一份存储上一次的内容，一份存储当前的内容，这样的话感觉整个下来，复杂度太高了
        * 我们能不能直接开放在渲染线程中，进行额外的Fence调用的机制，感觉上可以，这样的话，问题就简化一些了，逻辑上可以这样，我们在执行完某些操作之后，调一波Fence，这样的话来保证GPU一定执行结束了，这样的话全部操作都还是在渲染县城去执行，如果想要方便编写的话，可能一次的回调包会大一些，或者就是拆开来弄，但复杂度会高一些
        * 另外的话其实有一个简单的方式，就是直接开个子进程，然后再子进程中进行相应的计算，这么的话复杂度什么的一瞬间就都没了，比较舒服，代价的话就是对于一些比较简单的操作来说，会变成什么样
        * 重新梳理一下需求
            * 有的时候我们存在预烘焙或是保存之类的功能，这时需要一个回读功能，在这样的情况下，由于现在的FrameResource设计，使得无法比较简单的做到阻塞的功能
            * 这样的话我们通过制定回调的方式，来完成相应的同步功能，但是对于一些可能任务量较大的任务
* 这样的话，我们相当于只提供一个当运行结束时，阻塞等待渲染线程执行结束的同步操作，其余操作全部异步进行
    * 后续的话，我们或许可以提供额外的AdditionCommandQueue，用来做一些其他的辅助计算，但我们还是不期望在逻辑线程去访问渲染对象
* 对于资源创建，我们究竟应该如何处理？
    * 以现在的模型来看，我们在逻辑线程中，无法有效的访问到渲染线程内所创建的对象及资源，这使得我们需要构建一个中间对象，来辅助渲染线程与逻辑线程进行同步操作
    * 我们把这个中间对象称之为渲染代理，其生命周期的起始由逻辑线程触发构建，生命周期的终止由逻辑线程触发请求，当渲染线程执行到相应帧时，生命周期正式终止。同时触发相应的释放操作
        * 对象的释放是否可以做阻塞式，如果说可以完全做成阻塞式，那么的话就不需要有到当前帧的GPU任务执行完毕时的回调了
        * 现在GPU任务执行完毕时的回调应该全部都是为了资源释放而生成的，所以如果我们把资源释放做成同步的，那么的话就可以忽略掉这一波回调，
            * 资源有哪些情况
                * 由逻辑线程主导的资源，例如贴图，模型，窗口对应的SwapChain等，其创建与否，创建的形式，一般不收渲染线程的控制
                * 由渲染管线主导的资源，例如临时的RT，ShaderMap等
                    * 里面又有细分
                        * 可能会每帧创建的资源
                        * 持久存在的资源
                        * 与其他渲染代理相辅相成的资源
            * 资源的释放有那些情况
                * 有逻辑线程主导的资源
                    * 相应的资源/对象析构时触发
                * 由渲染管线主导的资源
                    * 对每帧会创建的资源，当当前帧对应的渲染任务执行结束后可以释放
                    * 当发现资源不再被需要时触发析构
* 渲染代理
    * 应该尽可能减少渲染代理的类型数量，在RenderGraph中，不存在对逻辑对象的访问，只存在对渲染代理的访问，所以化简渲染代理可以相当程度上降低新增RenderGraph的成本
    * 渲染代理分成两类，或者说渲染线程所持有的资源分为两类
        * GlobalRenderResources
        * FrameRenderResources
    * 渲染代理根据所从属于的Resource不同，会被分为相应的不同类型
        * 一种是不会为每个FrameResource储存不同数据的渲染代理 - RenderProxy
        * 一种是会为每个FrameResource储存不同数据的渲染代理 - FrameExclusiveRenderProxy
    * 仅对于FrameExclusiveRenderProxy，我们可以在逻辑线程中直接修改其内容，这得益于我们的同步机制
    * 不管对于RenderProxy还是FrameExclusiveRenderProxy来说，其生命周期都是固定的，都是在逻辑线程创建，渲染线程移除
        * 这样的话生命周期上会相对更可预测一些，所以在对资源池的数据访问上或许可以尝试一些不需要锁的手段，并借此避免不必要的阻塞，提高效率
* 现在我们都已经处理了什么
    * 我们完成了渲染线程和逻辑线程的同步异步操作
* 我们现在需要做什么
    * 我们现在需要完成渲染线程与逻辑线程之间的数据交互，才能进行下一步的渲染工作，并且在这个交互过程中，由于我们的同步机制的设计，使得我们需要引入一个可以贯穿渲染线程和逻辑线程的对象，我们将其称之为渲染代理
* 渲染代理的设计及需要设计的内容
    * 渲染代理的生命周期
        * 渲染代理其实一定程度上可以视作两个分别归属于渲染线程和逻辑线程的对象的重合，所以生命周期也就相应的是他们二者的并集，即在逻辑线程请求创建后生成，在逻辑线程请求释放后，在相应的渲染线程对应帧中释放
        * 在渲染代理的创建上，我们或许可以针对其生命周期，对其创建进行特定的优化，使得即使在异步访问下，也不需要进行频繁的同步操作
    * 渲染代理的分类及访问权限
        * 渲染代理分为两种类型，一种是会为每帧储存不同数据的渲染代理，一种是不会为每帧储存特定数据的资源
            * 其中会为每帧储存特定数据的渲染代理，是最主要的用来从逻辑线程向渲染线程传递数据的依托，得益于SE中的帧同步机制，我们可以没有在无锁且无访问冲突的情况下，直接修改当前帧下的渲染代理中的数据内容
            * 其中不会为每帧储存特定数据的资源，更多的是由于这两方面限制所造成的妥协产物，同时对于这类渲染代理来说，除了创建与释放操作外，其余的操作需要全部通过向渲染线程传递任务委托的方式来修改
                * 在逻辑线程中无法回读渲染线程的数据
                * 在逻辑线程中不期望直接创建/修改RDI资源
* 我们尚未进行的操作
    * 我们如何设定新增渲染代理时的接口
        * 
    * 我们如何设计GlobalRenderResource与FrameRenderResource，来更好的与渲染代理协同工作，并且允许自由的新增渲染代理
    * 我们可能需要部分重构TElementPool，以增加自动扩容功能
    * 我们期望可以构造出一套能够无锁且无异步访问冲突的渲染代理构造机制，并且我们期望这套机制是支持对象池的，相当多数的渲染代理可能都是会存在较多实例的小对象
    * 我们需要保证管线中可以简单便捷的访问到渲染代理，并且可以简单地获取到其中的数据及含义
    * 我们可能需要为渲染代理增加部分限制，来使得其遵守我们约定的访问规则，但这或许会引入新的代码复杂度
    * 我们或许需要为帧独占渲染代理进行额外的封装，以降低每次访问及修改时的复杂度
    * 我们需要在渲染线程中，引入阻塞等待全部GPU任务执行完毕的功能，以简化一些RDI对象的更新及创建，移除
* 太久没碰了，现在重新回溯一下我们都思考了什么
    * 首先我们定义了渲染代理的概念，以及渲染代理的分类，生命周期
    * 渲染代理的委托原则上会通过模板偏特化的方式来实现，以降低代码复杂度
    * 由于渲染代理的生命周期是明确的，所以，我们或许可以尝试更为高效的无锁编程，其甚至可以不依赖于原子量
    * 对元素构建上，我们可以先初步对ElementPool做一个特化，加个线程安全模式
    * 结构上的话，需要让渲染代理的可见性稍微高一些，需要让非渲染线程可见
* 执行
    * 先改造ElementPool，并提供win中的高效mutex锁
    * ElementPool需要有扩容的能力么，应该是需要的

* ElementPool扩容需要考虑的情况
    * 现在存在扩容时与非扩容时，单线程访问与多线程访问各两种会互相组合的情况
    * 单线程时都不是问题，核心是多线程时
        * 刚才突然想到的一个想法是期望对象获取尽量是快速而无锁的，对象获取应该只在扩容的时候才会发生相应的变更。
    * 扩容后应该依旧能正常的访问到相应的数据，所以在这里设置了代理的方式来访问，相当于指针了
    * 当相应的元素被删除后，应该也需要让访问无效化，或者说这个时候直接将其视作是野指针应该也可以，访问了未初始化的内存

    * 所以逻辑上应该差不多是这样
        * 增加池元素代理，以供外界访问
        * 对于原本的分配器等内容，增加自动扩容功能，并对算法进行相应的调整
        * 增加线程安全功能
        * 单纯的访问元素代理的时候，会发生问题的点主要是发生扩容的时候

    * 另外对于渲染代理的垃圾回收机制的维护，也生成了一套基本的思路
        * 在渲染线程与逻辑线程分别维护一套bitmap，以标明当前存活的渲染代理，这个bitmap不必区分帧独占数据等，只维护一套即可
        * 定期，例如隔30s，1000帧，或是当被移除的元素数量大于多少后，将数据传递过去，并对两个bitmap进行逐位操作，获取新被移除的元素
        * 对于添加操作，理论上应该是直接在逻辑线程进行创建即可，但是这个创建过程可能会生成

    * 这样的话带来一个新的操作，就是我们需要将原本的几个SCircularSlotAllocatorHelper之类的东西合并，并且生成一个bitmap类，用于后续的访问操作
    * 这里的本质实际上是我们需求的不再只是一个单纯的对象池，而是还需要访问到对象池内部的数据，以加速性能，所以我们才会有将一些东西拆出来的想法
    * 那么的话，对于资源池与bitmap，我们有没有更好的维护方法
    * 还有一个问题是感觉现在Core里面的内容会逐步的有膨胀的趋势，苗头不是很好
* Texture都分为什么
    * 静态Texture
    * 会运行时上传数据的Texture
    * RenderTarget
    * SwapChain中的贴图(这个在现在的框架中，或许可以忽略)
    * 会运行时调整大小的Texture？