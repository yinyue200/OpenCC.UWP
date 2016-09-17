using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.IsolatedStorage;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Storage;

namespace CsTest
{
    static class Common
    {
        public const int su_ok = 0;
        public const int su_fail = 1;
        public const int su_nullptr = 2;
        public const int su_overflow = 3;
        public const int su_unload = 4;
        public const int su_unknown = -1;

        public const int dic_type_one = 0x1;
        public const int dic_type_more = 0x2;
        public const int dic_type_fixed = 0x4;
        public const int dic_type_no_word = 0x8;

        public const int su_format_word = 0;
        public const int su_format_link = 1;
        public const int su_format_image = 2;
        public const int su_format_comment = 3;
        public const int su_format_book = 4;
        public const int su_format_ex1 = 5;
        public const int su_format_ex2 = 6;
        public const int su_format_ex3 = 7;
        public const int su_format_type_count = 8;

        public const string temp_folder = "";
        public const string data_folder = "data";

        static public async Task<bool> WriteFile(string folder, string fileName, byte[] content, int begin, int len)
        {
            bool ret = false;

            using (Stream stream = await OpenFileForWrite(folder, fileName))
            {
                if (stream != null)
                {
                    stream.Write(content, begin, len);
                    stream.Flush();
                    ret = true;
                }
            }

            return ret;
        }

        public static async Task<Stream> OpenFileForWrite(string folder, string file)
        {
            try
            {
                //  StorageFolder rootFolder = Windows.ApplicationModel.Package.Current.InstalledLocation;
                StorageFolder rootFolder = ApplicationData.Current.LocalFolder;
                StorageFolder dstFolder = rootFolder;
                if (folder != string.Empty)
                {
                    dstFolder = await rootFolder.CreateFolderAsync(folder, CreationCollisionOption.OpenIfExists);
                }

                if (dstFolder != null)
                {
                    StorageFile storageFile = await dstFolder.CreateFileAsync(file, CreationCollisionOption.ReplaceExisting);
                    return  await storageFile.OpenStreamForWriteAsync();
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }

            return null;
        }

        static public async Task<Stream> ReadFile(string folder, string fileName, bool installPos = true)
        {
            try
            {
                StorageFolder storageFolder = null;
                if (installPos) storageFolder = Windows.ApplicationModel.Package.Current.InstalledLocation;
                else storageFolder = ApplicationData.Current.LocalFolder;
                if (folder != string.Empty)
                    storageFolder = await storageFolder.GetFolderAsync(folder);
                StorageFile storageFile = await storageFolder.GetFileAsync(fileName);

                return await storageFile.OpenStreamForReadAsync();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                return null;
            }
        }

        public static async Task<byte[]> ReadBytesOfFile(string folder, string fileName)
        {
            using (Stream stream = await ReadFile(folder, fileName))
            {
                if (stream == null) return null;

                byte[] data = new byte[stream.Length];
                stream.Read(data, 0, (int)stream.Length);
                return data;
            }
        }
    }
}
