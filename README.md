# OpenCC.UWP

根据 http://www.cnblogs.com/lin277541/p/4928630.html 修改

基于 https://github.com/BYVoid/OpenCC 

使用 Apache License 2.0。

[![NuGet](https://img.shields.io/nuget/v/OpenCC.UWP.svg)](https://www.nuget.org/packages/OpenCC.UWP/)

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
