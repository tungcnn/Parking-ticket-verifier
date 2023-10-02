using IronOcr;

namespace car_rego_ticket_verifier
{
    public class CarTicketVerifierService
    {
        const string COPY_FROM_FPGA_COMMAND = "/C pscp -pw root -r root@192.168.7.1:/home/sit329/in/* C:/Users/sontu/OneDrive/Documents/SIT329-Group-5-Image-Processing/car-rego-ticket-verifier/files/in/";
        const string MOVE_FILE = "/C move ";
        const string MOVE_PROCESSED_FILE_TO = " ./files/processed";
        const string MOVE_ERROR_FILE_TO = " ./files/error";
        public async static Task<Dictionary<string, string>> extractRegoFromImage()
        {
            //Import images from FPGA
            await runTerminalCommand(COPY_FROM_FPGA_COMMAND);

            //Extract info from images
            return getRegoAndTicketFromImage();
        }

        //Helper fucntion to run terminal commands to move images around
        private async static Task runTerminalCommand(string command)
        {
            await Task.Run(() =>
            {
                System.Diagnostics.Process process = new System.Diagnostics.Process();
                System.Diagnostics.ProcessStartInfo startInfo = new System.Diagnostics.ProcessStartInfo();
                startInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                startInfo.FileName = "cmd.exe";
                startInfo.Arguments = command;
                process.StartInfo = startInfo;
                process.Start();
            });
        }

        //Extract rego and ticket from received images
        private static Dictionary<string, string> getRegoAndTicketFromImage()
        {
            Dictionary<string, string> map = new Dictionary<string, string>();

            try
            {
                string[] files = Directory.GetFiles(@"./files/in");

                if (files != null && files.Length > 0)
                {
                    foreach (string file in files)
                    {
                        //Extract rego
                        OcrResult ocrResult = new IronTesseract().Read(file);
                        string rego = ocrResult.Text;
                        if (rego == null || rego == "")
                        {
                            runTerminalCommand(MOVE_FILE + file + MOVE_ERROR_FILE_TO);
                            continue;
                        }

                        rego = cleanUpRego(rego);

                        ////Extract Barcode
                        //string[] results = BarcodeReader.Read(file, null).Values();

                        //string ticket;

                        //if (results != null && results.Length > 0)
                        //{
                        //    ticket = results[0];
                        //}
                        //else
                        //{
                        //    runTerminalCommand(MOVE_FILE + file + MOVE_ERROR_FILE_TO);
                        //    continue;
                        //}

                        //Add extracted rego and ticket to a Dictionary list, with rego as Key and ticket as Value
                        map.Add(rego, "tickerNumber");
                        runTerminalCommand(MOVE_FILE + file + MOVE_PROCESSED_FILE_TO);
                    }
                }
            }
            catch (Exception ex)
            {
                Console.Write(ex.ToString());
            }

            return map;
        }

        private static string cleanUpRego(string rego)
        {
            rego = rego.Replace(" ", string.Empty);
            rego = rego.Replace("\r", string.Empty);
            rego = rego.Replace("\n", string.Empty);
            rego = rego.Replace("°", string.Empty);
            rego = rego.Replace("eeeaaa|", string.Empty);
            rego = rego.Replace("SOUTHAUSTRALIA", string.Empty);
            rego = rego.Replace("VICTORIATHEEDUCATIONSTATE", string.Empty);
            rego = rego.Replace("-", string.Empty);
            rego = rego.Replace("‘", string.Empty);

            return rego;
        }
    }
}
