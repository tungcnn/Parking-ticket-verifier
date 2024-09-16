using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;

public class Datalist
{
    [BsonId]
    public ObjectId Id { get; set; }
    public string rego { get; set; } = null! ;
    public string QRstring { get; set; } = null!;
}
