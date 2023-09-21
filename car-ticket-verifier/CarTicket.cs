using MongoDB.Bson.Serialization.Attributes;
using MongoDB.Bson;

namespace car_ticket_verifier
{
    internal class CarTicket
    {
        [BsonId]
        public ObjectId Id { get; set; }
        public string rego { get; set; } = null!;
        public string QRstring { get; set; } = null!;
    }
}
