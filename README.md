# OpenCC.UWP

根据 http://www.cnblogs.com/lin277541/p/4928630.html 修改

基于 https://github.com/BYVoid/OpenCC 

使用 Apache License 2.0。

##我的使用方法

可能大家有更好的办法，欢迎告诉我。以下方法假设你自己的项目采用C#语言

使用XML编辑器打开项目（你自己的项目）的csproj文件，在所有含有Condition属性且Condition属性中包含“$(Platform)”的PropertyGroup节点中加入

如果Condition属性中包含ARM，请在节点中添加子节点

`<cppPlatformTarget>ARM</cppPlatformTarget>`

包含X64，添加子节点

`<cppPlatformTarget>x64</cppPlatformTarget>`

包含x86,添加子节点

`<cppPlatformTarget>Win32</cppPlatformTarget>`

然后保存并关闭文件

这时打开下载好的本项目源代码，编译。

返回源项目（你自己的项目），在项目（不是解决方案）根目录中添加现有项，找到opencc解决方案根目录下的Release中的x64（推荐Release文件夹，前提你编译使是Release编译）文件夹，选择OpenCCx.dll，然后在添加按钮右边的小三角中选择“添加为链接”

然后再打开你自己项目的csproj文件夹，找到包含OpenCCx.dll的Content节点，把其中的x64改成$(cppPlatformTarget)

然后按同样方法添加对OpenCCxRuntineComponent.winmd的引用，注意这时是添加引用而不是添加现有项，和上次一样添加后记的替换路径中的x64什么的为$(cppPlatformTarget)

然后就完成了

使用例子

`OpenCCxRuntineComponent.OpenCCx opencc_s2t = new OpenCCxRuntineComponent.OpenCCx();//創建`
`opencc_s2t.Load(OpenCCxRuntineComponent.ConverterType.Simple2Traditional);//使用什麼轉換規則`


`

private void btn2Simple_Click(object sender, RoutedEventArgs e)

        {
            string ret = opencc_t2s.Convert(textBox.Text);//使用簡單的Convert就OK了。
            if (ret != null)
            {
                textBox.Text = ret;
            }
        }
`
