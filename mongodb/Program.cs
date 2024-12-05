using MongoDB.Driver;
using System;
using System.Threading.Tasks;
using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;

namespace Data{
    class Program
    {
        static async Task Main(string[] args)
        {
            // MongoDB connection string and database name
            string connectionString = "mongodb+srv://michaellaiwz:W4EAap59qOAGcIH0@cluster0.dpctlis.mongodb.net/?retryWrites=true&w=majority";
            string databaseName = "SIT329";
            string collectionName = "cars";

            // Initialize a MongoClient
            var client = new MongoClient(connectionString);

            // Access the database
            var database = client.GetDatabase(databaseName);

            // Access the collection 
            var collection = database.GetCollection<Datalist>(collectionName);

            // Define the search criteria (rego and QRstring)
            string searchRego = "aad13321ad";
            string searchQRstring = "QRcodetest";

            // Create a filter to match documents with the provided rego and QRstring
            var filter = Builders<Datalist>.Filter.And(
                Builders<Datalist>.Filter.Eq("rego", searchRego),
                Builders<Datalist>.Filter.Eq("QRstring", searchQRstring)
            );

            // Use the filter to find documents matching the criteria
            var matchingDocuments = await collection.Find(filter).ToListAsync();

            // Check if any documents match the criteria
            bool foundMatch = matchingDocuments.Count > 0;

            Console.WriteLine($"Matching documents found: {foundMatch}");

            // Close the application
            Console.WriteLine("Press any key to exit...");
            Console.ReadKey();
        }
        }
}
