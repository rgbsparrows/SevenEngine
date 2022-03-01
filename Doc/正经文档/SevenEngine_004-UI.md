* UI模块会以Imgui为基础进行重新构建
* Imgui分为两部分，一部分是偏向于逻辑的gui构建，一部分是偏向于渲染的ImWindow等对象的构建
    * 结构上应该类似于这样
        * UIContext
            * 内含OnGui函数，用来完成Imgui的各种调用操作
            * UIContext应该能够依赖于反射机制，以完成数据驱动
        * UIWindow
            * 用于实际创建的窗口类，以便于与渲染线程等进行交互

* 开发流程
    * 