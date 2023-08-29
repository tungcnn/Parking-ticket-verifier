using System.Drawing;

namespace ImageToBinaryConverter
{
    class Program
    {
        static void Main(string[] args)
        {
            FileInfo fileInfo = new FileInfo("./car1.jpg");

            // The byte[] to save the data in
            byte[] data = new byte[fileInfo.Length];

            // Load a filestream and put its content into the byte[]
            using (FileStream fs = fileInfo.OpenRead())
            {
                fs.Read(data, 0, data.Length);
            }

            File.WriteAllBytes("./output.txt", data);
        }
    }
}