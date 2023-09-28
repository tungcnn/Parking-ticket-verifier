using Microsoft.EntityFrameworkCore;

namespace car_rego_ticket_verifier
{
    public class CarTicketDB : DbContext
    {
        public CarTicketDB(DbContextOptions<CarTicketDB> options) : base(options) { }

        public DbSet<CarTicket> CarTickets => Set<CarTicket>();
    }
}
