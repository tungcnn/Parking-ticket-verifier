// Function to fetch and display a single car ticket by registration number
async function fetchCarTicketByRego() {
    try {
        const response = await fetch(`https://localhost:7087/verify-car-ticket`, {
            mode: 'cors',
            method: 'GET'
        }).then((response) => response.json())
        .then((json) => {
            const statusCircle = document.getElementById('statusCircle');
            const statusText = document.getElementById('statusText');

            statusCircle.style.backgroundColor = '#6bff6b'; // Set background color to green
            statusText.innerText = json.rego;
        }).catch(() => {
            statusCircle.style.backgroundColor = '#ff6b6b'; // Set background color to red
            statusText.innerText = 'Car ticket not found';
        });
    } catch (error) {
        console.error('Error fetching car ticket:', error);
    }
}

// Call the fetchCarTicketByRego function with a specific registration number
window.onload = () => {
    fetchCarTicketByRego();
};
