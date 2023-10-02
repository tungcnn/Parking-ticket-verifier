using car_rego_ticket_verifier;
using Microsoft.EntityFrameworkCore;

var builder = WebApplication.CreateBuilder(args);
builder.Services.AddDbContext<CarTicketDB>(opt => opt.UseInMemoryDatabase("CarTicketList"));
builder.Services.AddCors();
//builder.Services.AddDatabaseDeveloperPageExceptionFilter();
var app = builder.Build();
app.UseCors(builder => builder.AllowAnyOrigin().AllowAnyHeader().AllowAnyMethod());

//Seed data
app.MapGet("/seed-car-tickets", async (CarTicketDB carTicketDb) =>
{
    CarTicket car1 = new CarTicket("BOMB911", "tickerNumber");
    carTicketDb.CarTickets.Add(car1);
    CarTicket car2 = new CarTicket("AAA111", "tickerNumber");
    carTicketDb.CarTickets.Add(car2);
    await carTicketDb.SaveChangesAsync();

    return Results.Ok();
});

//Verify car ticket
app.MapGet("/verify-car-ticket", async (CarTicketDB carTicketDb) =>
{
    Dictionary<string, string> result = await CarTicketVerifierService.extractRegoFromImage();

    CarTicket? foundCarTicket = null;

    if (result.Count() > 0)
    {
        foreach (var car in result)
        {
            var resultList = await carTicketDb.CarTickets.Where(x => x.Rego == car.Key
                          && x.Ticket == car.Value)
                          .ToListAsync();

            if (resultList.Count() > 0)
            {
                foundCarTicket = resultList[0];
            }
        }
    }

    if (foundCarTicket == null)
    {
        return Results.NotFound();
    }
    else
    {
        return Results.Ok(foundCarTicket);
    }
});

//Get all
app.MapGet("/car-tickets", async (CarTicketDB carTicketDb) =>
    await carTicketDb.CarTickets.ToListAsync());

//Get one by rego
app.MapGet("/car-tickets/{rego}", async (string rego, CarTicketDB carTicketDb) =>
    await carTicketDb.CarTickets.FindAsync(rego)
        is CarTicket carTicket
            ? Results.Ok(carTicket)
            : Results.NotFound());

//Insert
app.MapPost("/car-tickets", async (CarTicket carTicket, CarTicketDB carTicketDb) =>
{
    carTicketDb.CarTickets.Add(carTicket);
    await carTicketDb.SaveChangesAsync();

    return Results.Created($"/car-tickets/{carTicket.Rego}", carTicket);
});

//Update by rego
app.MapPut("/car-tickets/{rego}", async (string rego, CarTicket carTicketNew, CarTicketDB carTicketDb) =>
{
    var carTicketOld = await carTicketDb.CarTickets.FindAsync(rego);

    if (carTicketOld is null) return Results.NotFound();

    carTicketOld.Rego = carTicketNew.Rego;
    carTicketOld.Ticket = carTicketNew.Ticket;

    await carTicketDb.SaveChangesAsync();

    return Results.NoContent();
});

//Delete by rego
app.MapDelete("/car-tickets/{rego}", async (string rego, CarTicketDB carTicketDb) =>
{
    if (await carTicketDb.CarTickets.FindAsync(rego) is CarTicket carTicket)
    {
        carTicketDb.CarTickets.Remove(carTicket);
        await carTicketDb.SaveChangesAsync();
        return Results.NoContent();
    }

    return Results.NotFound();
});

app.Run();