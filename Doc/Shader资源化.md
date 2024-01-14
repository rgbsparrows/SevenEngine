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
        * 现在的Inputlayout，Shader是以资源形式存在的
            * 不保留这样的形式，如果后面要做多底层API支持的话，把这些东西都存在PipelineState中隐式储存
    * ShaderManager
    ``` cpp
    ```
* RDI部分简化
    * 移除InputLayout，Rootsignature，Shder对象
        * 创建与Set
    * 一些可缓存的相关机制需要提前确认
    * Shader编译行为交给Factory进行
* RShader只返回Pipelinestate对象
* Shader对象的缓存由Pipelinestate对象自行处理

* 期盼
    1. Shader本身对ShaderInstance隐藏
    2. Shader最好能够支持刷新机制
        * 刷新机制中，既包括对Shader的修改，包含对默认管线状态与默认编译宏的修改
* XX
    * Shader
        * Shader本身，出于优化的角度考虑，应当将PSO缓存与Shader缓存相分开，其对于ShaderBlob的缓存，仅应当作为Shader对象的内部优化，不应对外公开
    * ShaderInstance
        * ShaderInstance不建议访问到全部的管线状态Desc，同时，Shader的BufferView其逻辑上也没有机会访问到
        * 所以，单独将需要访问的部分概念下状态与Shader编译宏抽出，其作为Shader实例化时实际发生的变更的数据来源
    * RShaderInstanceParameter
    ``` cpp

    struct RCachedPSO
    {
        RShaderInstanceParameter parameter;
        RDIPipelineState* pso;
    }

    struct RCachedShader
    {
        //SRDIShaderMacro mShaderMacro;
        SBlob mCompiledShader;
	    SRange mVsRange;
	    SRange mHsRange;
	    SRange mDsRange;
	    SRange mGsRange;
	    SRange mPsRange;
    }

    class RShader
    {

        map<RShaderInstanceParameterIdentify, CachedPSO> psoMap;

        map<shaderIdentify, RCachedShader> CachedShaderMap;

        XXX MAKEpso(RShaderInstanceParameter PARAM);

        void ForceRefrash()
        {
            // Load

            shaderIdentify = GenerateByLoadInfo();

            map<RShaderInstanceParameterIdentify, CachedPSO> rawcachedPso = psoMap;

            CachedShaderMap.clear();
            psoMap.Clear();

            for (auto _ele : rawcachedPso)
            {
                MAKEpso(_ELE.SECTION.parameter);
            }

        }

        private MergePS(LEFT, RIGHT);

        RShaderIdentify = shaderIdentify;

    }


    enum class EShaderInstanceParameterEle
    {
        ShaderMacro,
        InputLayout,
        PrimitiveTopologyType,
        XXX
    };
    
    REGIST_ENUM_FLAG_FORM_ENUM(EShaderInstanceParameterFlag, EShaderInstanceParameterEle)

    struct RShaderInstanceParameter
    {
        SRDIShaderMacro mShaderMacro;
	    SRDIVertexInputLayout mInputLayout;
	    ERDIPrimitiveTopologyType mPrimitiveTopologyType = ERDIPrimitiveTopologyType::TRIANGLE;
	    SRDIBlendState mBlendState;
	    SRDIRasterizationState mRasterizationState;
	    SRDIDepthStencilState mDepthStencilState;
	    uint32_t mRenderTargetCount = 1;
	    ERDIPixelFormat mRenderTargetFormat[8] = {};
	    ERDIPixelFormat mDepthStencilFormat = ERDIPixelFormat::D24_UNORM_S8_UINT;

        EShaderInstanceParameterFlag mFlag = EShaderInstanceParameterFlag::None;
    }

    using RShaderInstanceParameterIdentify = TTypeHandle<CalcStrHash("shaderparameter"), uint64_t>;

    RShaderInstanceParameterIdentify MakeShaderInstanceParameterIdentify(const RShaderInstanceParameter& _param)
    {
        if (_param & ConvertToEnumFlag(EShaderInstanceParameterEle::ShaderMacro))
        {
            XX;
        }
    }
    

    class RShaderInstance
    {
        RShaderInstanceParameterIdentify mIdentify;
        RShaderInstanceParameter mCacheShaderInstanceParameter;
        IRDIGraphicsRootSignature* mCacheRootSignature;
    }
    ```

* 流程上应该是这样
    * 正常渲染执行
        * ShaderInstance创建，指定Shader，指定Affector
        * ShaderInstance构造时，如果指定Shader未加载，那么加载对应的Shader
        * Affector获取Shader的默认MicorPipelineState，并在此基础上修改，同时指定Flag，标识自己修改了哪些内容
        * 将修改后的MicroPipelineState传入Shader的MakePso函数当中，MakePso函数返回对应的Identify以及PSO对象，MicroPipelineState数据
            * 在创建PSO对象时，其会存储传入的MicroPipelineState的对象内容（可能会清空未标记Flag的部分，以避免可能的污染问题）
            * 假设失败，那么Identify为Identify::Error
        * 渲染时，访问ShaderInstance，直接获取对应对象及MicroPSO数据，并据此完成渲染工作，或是在存在问题时报错/跳过某些步骤
    * Shader强制刷新
        * 刷新工作由ShaderManager触发，其根据传入指令的不同，决定刷新部分/全部Shader
        * Shader的实际刷新工作在RShader::Refrash中完成
            * 其内部进行两件事
                1. 更新Shader的标识，以间接通知ShaderInstance，其BaseShader发生了变更
                    * 标识怎么生成以后再想，主要的复杂点在于如果要做Shader的硬盘缓存与加载，那么标识怎么能有效的标识信息
                2. 暂存MicroPipelineState数据后清空全部Shader，PSO对象，并根据数据重新生成PSO对象，以及间接生成Shader对象
            * 如果有硬盘缓存机制
                * 那么还需要增加对Shader硬盘存储内容的刷新
    * Shader的硬盘缓存与加载工作
        * 数据直接存，存完了加载
        * 核心在于刷新与数据标识
            * 这部分内容之前没有考虑过
            * 核心问题在于有效的标识数据ID

    * MicroPipelineState
