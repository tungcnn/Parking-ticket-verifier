using IronBarCode;
using IronOcr;
using System.Runtime.Versioning;
using MongoDB.Driver;
using MongoDB.Bson;

namespace car_ticket_verifier
{
    [SupportedOSPlatform("windows")]
    internal class Program
    {
        const string connectionUri = "mongodb+srv://michaellaiwz:W4EAap59qOAGcIH0@cluster0.dpctlis.mongodb.net/?retryWrites=true&w=majority";
        const string COPY_FROM_FPGA_COMMAND = "/C pscp -pw root -r root@192.168.7.1:/home/in C:/Users/sontu/OneDrive/Documents/SIT329-Group-5-Image-Processing/car-ticket-verifier/files";
        const string MOVE_FILE = "/C move ";
        const string MOVE_PROCESSED_FILE_TO = " ./../../../files/processed";
        const string MOVE_ERROR_FILE_TO = " ./../../../files/error";

        static async Task Main(string[] args)
        {
            var settings = MongoClientSettings.FromConnectionString(connectionUri);
            // Set the ServerApi field of the settings object to Stable API version 1
            settings.ServerApi = new ServerApi(ServerApiVersion.V1);
            // Create a new client and connect to the server
            var client = new MongoClient(settings);
            var database = client.GetDatabase("SIT329");
            var collection = database.GetCollection<CarTicket>("cars");

            while (true)
            {
                //Import images from FPGA
                runTerminalCommand(COPY_FROM_FPGA_COMMAND);

                //Extract info frm images
                Dictionary<string, string> cars = getRegoAndTicketFromImage();

                //Verify extracted info
                if (cars.Count > 0)
                {
                    foreach (var car in cars)
                    {
                        var filter = Builders<CarTicket>.Filter.And(
                          Builders<CarTicket>.Filter.Eq("rego", car.Key),
                          Builders<CarTicket>.Filter.Eq("QRstring", car.Value)
                        );

                        CarTicket result = await collection.Find(filter).FirstOrDefaultAsync();

                        if (result != null)
                        {
                            //TO-DO: PRINT SUCESS RESULT
                        } else
                        {
                            //TO-DO: PRINT FAILED RESULT
                        }
                    }
                }
            }
        }

        //Helper fucntion to run terminal commands to move images around
        private static bool runTerminalCommand(string command)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            System.Diagnostics.ProcessStartInfo startInfo = new System.Diagnostics.ProcessStartInfo();
            startInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
            startInfo.FileName = "cmd.exe";
            startInfo.Arguments = command;
            process.StartInfo = startInfo;
            return process.Start();
        }

        //Extract rego and ticket from received images
        private static Dictionary<string, string> getRegoAndTicketFromImage()
        {
            Dictionary<string, string> map = new Dictionary<string, string>();

            try
            {
                string[] files = Directory.GetFiles(@"./../../../files/in");

                if (files != null && files.Length > 0) 
                {
                    foreach (string file in files)
                    {
                        //Extract rego
                        string rego = new IronTesseract().Read(file).Text;
                        if (rego == null || rego == "")
                        {
                            runTerminalCommand(MOVE_FILE + file + MOVE_ERROR_FILE_TO);
                            continue;
                        }

                        //Extract Barcode
                        string[] results = BarcodeReader.Read(file, null).Values();

                        string ticket;

                        if (results != null && results.Length > 0)
                        {
                            ticket = results[0];
                        }
                        else
                        {
                            runTerminalCommand(MOVE_FILE + file + MOVE_ERROR_FILE_TO);
                            continue;
                        }

                        //Add extracted rego and ticket to a Dictionary list, with rego as Key and ticket as Value
                        map.Add(rego, ticket);
                        runTerminalCommand(MOVE_FILE + file + MOVE_PROCESSED_FILE_TO);
                    }
                }
            }
            catch (Exception ex)
            {
            }

            return map;
        }
    }
}