* 大部分内容应该是可选有默认值的
    * 部分东西可能需要是动态的

* Shader本身
    * 当前包含的Shader类型
        * VS|PS|MS|XXX
* RootSignature
    * RootParameter
    * Sampler
* InputLayout
* PrimitiveTopologyType
    * 可选，TRIANGLE
* BlendState
    * 可选，One Zero One Zero
* RS
    * 可选
* Depth
    * 可选
        * DepthBias等可能需要是动态的
* RTFormat
    * 有默认值
* DSFormat
    * 有默认值

* 一部分内容，是必须要填写的，其本身信息也与Shader强绑定
* 一部分信息大部分情况下可以不填写，并有默认值
* 一部分信息需要额外指定的

* 所以Shader本身对应的资源，应该是类似Shader模板一类的存在，而后，会有其他的Shader实例，引用Shader对象，并对其参数进行额外赋值，例如DepthBias

* 对象类型
* IShaderInterface
    * Shader
    * ShaderInstance
* 渲染时应该能够接受二者任意一个，即存在统一基类
* Shader描述文件与Shader本身大概率需要分开，但这个分开主要是便于编写
* 渲染过程中，是存在多个Renderer
    * Shader模板需要共用的
        * Shader模板加载，管理，卸载，引用维护的代码流程
            * 在Resource里是已经有一份的
* 另一个选项
    * 让ResourceModule支持多线程并发访问
        * 整体上应该可控
        * 会不会有什么潜在问题呢？
            * 一部分资源对象实际上隐式包含了自身在GameThread的逻辑
                * 例如Mesh资源，其包含了上传逻辑，内部隐含了线程归属权问题
            * 如果对Render开放ResourceModule，那么可能会在这些资源上导致问题
    * 在渲染线程再独立写一份ShaderManager？
        * 九成概率是这么写，越写越像UE
    * 在逻辑线程完成Renderer的部分初始化
        * 这样的话可以正常的使用ResourceModule
        * 但是编写的复杂度会提高一些
            * Renderer的很多数据需要跨线程被访问
* 重新梳理诉求
    * 在渲染线程能加载，访问Shader
    * 一部分Shader或许也需要再逻辑线程访问
    * 渲染线程期望对Shader有较大的，不需要等待的控制权
        * 对后处理等效果，也可以将其材质化
        * 一部分差异可以通过渲染线程的config属性来调整
    * 材质本身一定和逻辑线程强相关
        * 但Shader未必
    * 问题
        * Shader可以在哪个线程，必须在哪个线程？
        * Manager
            * 现在其实已经有Class，Module，Resource这几个Manager了
            * 再加个Shader可能也行
            加吧
            * 差异是什么？
        * Class
            * 内部全部数据都是固定的，并且不会在动态插入/移除
        * Module
            * 需要增加动态卸载/加载功能
        * Resource
            * 需要增加动态卸载/加载功能，对资源的修改
            * 序列化与反序列化
            * 创建等功能
        * Shader
            * 
* Shader文件与Shader实例的加载流程
    1. 是否要走正常的资源加载逻辑？还是管线资源单独享有一套资源加载逻辑？
        * 现在资源流程是单线程的，如果走异步可能会有问题，但如果能直接在现在的基础上弄，或许会方便一些
        * 如果在现在的基础上弄，可能会Shader也会像Mesh资源一样，事实上存在两份资源
        * 
* 除了Shader，贴图是否也有类似问题？
* 暂时倾向
    * 编写ShaderManager，但隐藏其接口
    * 对外提供这样的接口形式
    ``` cpp
    RShader xxx("Engine/Shader/xxx.shader")
    RShaderIns xx1(xxx);
    RShaderIns xx2("Engine/Shader/xxx.shader");

    //MaterialShader
    RMaterialShader xxx("Engine/Shader/xxx.mshader")
    RMaterialShaderIns xxx2("Engine/Shader/xxx.mshader")
    ```
    * 功能拆分
        * ShaderManager
            * Shader加载功能
                * Shader加载
                * 应当有强制刷新部分/全部Shader的功能
        * Shader实例与接口
            * Shader
                * 缓存功能
                    * 缓存由Shader实例所触发的Shader编译缓存
                    * 缓存由Shader实例所触发的PSO生成
                    * Shader编译与PSO缓存的自动清理工作应当能自动延迟三帧
                    * Shader与PSO的缓存应互相独立
                        * 可能也需要代理对象？
                        * 一个期盼
                            * 资源管理尽量依赖于RAII进行，对外不提供addref等接口
                * 完整的默认值与参数映射
            * Shader实例
                * 允许额外指定编译宏
                * 允许重新指定PSO内其他数据
                * 当重新指定后，应当由刷新PSO的功能
                    * 如果刷新PSO，那么就有渲染与GPU的同步问题
                        * 预期是尽量不造成阻塞
                * PSO生成时机
                    * 在Shader被实际调用时发生
                        * 延迟构造
                    * 手动调用PSO刷新
                    * 在ShaderInstance构造时产生
                        * 倾向于此
                * Shader实例的构造参数？
                    * Shader本身
                    * Shader实例应当允许派生
                    ``` cpp
                    // 可Private隐藏
                    class RShader final
                    {
                        public:
                            spsostate GetPsoState() const {xxx;}

                            xxx GetOtherInfo() const;
                        public:
                            void MakePso_Internal(Macro, PSOState);

                        private:
                            size_t refCount;
                    }



                    // ShaderInstance主要是重新指定Macro与PSO，所以只要输入RShader，输出Macro与PSO即可
                    // 起个名 Shader-》ShaderAffector -》ShaderInstance

                    using RShaderAffector = std::function<void(const RShader&, Macro&, PSO&)>;

                    class RShaderInstance
                    {
                    public:
                        RShaderInstance(RShader& Shader, RShaderAffector xxx = DefaultShaderAffector());
                        RShaderInstance(std::filesystem::path shaderPath, RShaderAffector xxx= DefaultShaderAffector());
                        RShaderInstance(SShaderIdentify SahderIdentify, RShaderAffector xxx= DefaultShaderAffector());

                        void GetPso();
                        void GetInputLayout();

                    protected:
                        PSO* PSO;
                        OtherInfo;
                    private:
                        RShader* BaseShaders;
                    }

                    struct SDefferShaderAffector
                    {
                        SDefferShaderAffector(bool isMobile);

                        void operator()(cosnt SShader& shader,Macro outmacro, PSO outpso)
                        {
                            if (isMobile)
                                 pso.rtcoutn = 3;
                            else pso.rtcoutn = 5;
                        }
                    }

                    RShaderInstance defferShader("Engine/Shader/deffer.shader", SDefferShaderAffector(GIsMobile));

                    SetShader(CommandList, defferShader);
                    {
                        vsinput  = deffershader->GetvSiNPUT;
                        IF (VSINPUT->HASnORMAL())
                        {
                            COMMANDLIST->SETVERTEXBUFFER(S);
                        }
                    }

                    RShaderInstance defferShader("Engine/Shader/deffer.shader", [&](const RShader&, Macro&, PSO&)
                    {
                        if (GIsMobile)
                            pso.rtcount = 3;
                         else pso.rtcount = 7;
                    });

                    RShaderInstance tonemapping("Engine/Shader/tonemapping.shader")

                    ```
        * Shader本身是否允许直接使用，还是必须通过Shader实例来使用
            * 实质问题是Shader本身是否提供足够的有效默认值
            * 问题不止在于默认值，还有对象的引用关系
            * 预期值通过Shader实例访问
        * 如果直接继承ShaderInstance，那么ShaderInstance里很多功能在子类编写时都要支持，或许相对麻烦一些
        * PSOCache
            * 相关信息应该有两个
                * 当前引用数量
                * 距离死亡的帧数
    * ShaderManager
    ``` cpp
    ```
