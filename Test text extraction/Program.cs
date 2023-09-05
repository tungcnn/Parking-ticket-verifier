using IronOcr;
using System.Runtime.Versioning;
using IronBarCode;

//Download IronOcr and IronBarCode first for this to work
namespace Test_text_extraction
{
    [SupportedOSPlatform("windows")]
    internal class Program
    {
        static void Main(string[] args)
        {
            string[] files = Directory.GetFiles(@"./../../../images/");

            foreach (string file in files)
            {
                //Extract rego
                string text = new IronTesseract().Read(file).Text;
                if (text != null && text != "")
                {
                    Console.WriteLine("Rego from " + file + ": " + text);
                }
                else
                {
                    Console.WriteLine("Cannot read rego from image " + file);
                }

                //Extract Barcode
                BarcodeResults results = BarcodeReader.Read(file, null);
                foreach (BarcodeResult result in results)
                {
                    Console.WriteLine("Bar Code from " + file + ": " + result.Text);
                }
            }
        }
    }
}