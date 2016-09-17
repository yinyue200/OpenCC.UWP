using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

//“空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409 上有介绍

namespace CsTest
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        OpenCCxRuntineComponent.OpenCCx opencc_s2t = new OpenCCxRuntineComponent.OpenCCx();
        OpenCCxRuntineComponent.OpenCCx opencc_t2s = new OpenCCxRuntineComponent.OpenCCx();

        public MainPage()
        {
            this.InitializeComponent();

            Load();
        }

        private void Load()
        {
            opencc_s2t.Load(OpenCCxRuntineComponent.ConverterType.Simple2Traditional);
            opencc_t2s.Load(OpenCCxRuntineComponent.ConverterType.Traditioan2Simple);

            string err1 = opencc_s2t.Error();
            string err2 = opencc_t2s.Error();
        }

        private void btn2Simple_Click(object sender, RoutedEventArgs e)
        {
            string ret = opencc_t2s.Convert(textBox.Text);
            if (ret != null)
            {
                textBox.Text = ret;
            }
        }

        private void btn2Traditioal_Click(object sender, RoutedEventArgs e)
        {
            string ret = opencc_s2t.Convert(textBox.Text);
            if (ret != null)
            {
                textBox.Text = ret;
            }

            //convert();
        }
        /*/// <summary>
        /// 源作者写的，没看懂干什么，遂删除
        /// </summary>
        private async void convert()
        {
            Stream s = await Common.ReadFile("opencc", "TWPhrasesAll.txt");
            StreamReader r = new StreamReader(s);
            string all = r.ReadToEnd();
            string[] a_all = all.Split(new char[] { '\n' });

            Stream ws = await Common.OpenFileForWrite("opencc", "TW2SPhrasesAll.txt");
            StreamWriter ww = new StreamWriter(ws);
            foreach(string item in a_all)
            {
                if (item.Trim() == string.Empty) continue;

                string[] a_item = item.Split(new char[] { '\t' });
                string ft = a_item[0];
                string tw = a_item[1];

                string jt = opencc_t2s.Convert(ft);

                string[] a_tw = tw.Split(new char[] { ' ' });
                foreach(string tw_item in a_tw)
                {
                    string newtext = tw_item + "\t" + jt + "\n";
                    ww.Write(newtext);
                }
            }

            ww.Flush();
        }*/
    }
}
