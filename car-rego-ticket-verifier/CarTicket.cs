using System.ComponentModel.DataAnnotations;

namespace car_rego_ticket_verifier
{
    public class CarTicket
    {
        [Key]
        public string Rego { get; set; } = "";
        public string Ticket { get; set; } = "";

        public CarTicket(string rego, string ticket) {
            this.Rego = rego;
            this.Ticket = ticket;
        }
    }
}
