// Function to fetch and display a single car ticket by registration number
async function fetchCarTicketByRego(rego) {
    try {
        const response = await fetch(`/car-tickets/${rego}`); // Fetch data from API
        const statusCircle = document.getElementById('statusCircle');
        const statusText = document.getElementById('statusText');

        if (response.status === 404) {
            // Handle the case where the car ticket is not found
            statusCircle.style.backgroundColor = '#ff6b6b'; // Set background color to red
            statusText.innerText = 'Car ticket not found';
        } else if (response.status === 200) {
            // Parse and display the car ticket data
            statusCircle.style.backgroundColor = '#6bff6b'; // Set background color to green
            const carTicket = await response.json();
            statusText.innerText = carTicket.ticket;
        } else {
            console.error('Error fetching car ticket:', response.status);
        }
    } catch (error) {
        console.error('Error fetching car ticket:', error);
    }
}

// Call the fetchCarTicketByRego function with a specific registration number (e.g., 'BOMB911')
window.onload = () => {
    fetchCarTicketByRego('BOMB911'); // Replace 'BOMB911' with the registration number you want to fetch
};
