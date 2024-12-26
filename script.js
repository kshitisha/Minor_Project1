document.addEventListener('DOMContentLoaded', () => {
    
    setupImagePreview('originalImage', 'originalPreview');
    setupImagePreview('suspectImage', 'suspectPreview');

    
    const forgeryForm = document.getElementById('forgeryForm');
    forgeryForm.addEventListener('submit', async (event) => {
        event.preventDefault();

        const formData = new FormData(forgeryForm);
        const resultContainer = document.getElementById('resultContainer');
        resultContainer.classList.add('hidden');
        resultContainer.innerHTML = 'Detecting...';

        try {
            const response = await fetch('/detect-forgery', {
                method: 'POST',
                body: formData,
            });

            if (!response.ok) {
                throw new Error('Failed to detect forgery');
            }

            console.log('Response Status:', response.status);  

            const result = await response.json();
            console.log('Detection result:', result);  
         
            displayResult(result);
        } catch (error) {
            console.error('Error during forgery detection:', error);
            resultContainer.innerHTML = 'An error occurred. Please try again.';
        }
    });
});

/**
 * Set up preview functionality for an image input
  @param {string} inputId - The ID of the input element
  @param {string} previewId - The ID of the image preview element
 */
function setupImagePreview(inputId, previewId) {
    const input = document.getElementById(inputId);
    const preview = document.getElementById(previewId);

    if (input && preview) {
        input.addEventListener('change', (event) => {
            const file = event.target.files[0];
            if (file) {
                const reader = new FileReader();
                reader.onload = (e) => {
                    preview.src = e.target.result;
                    preview.classList.remove('hidden');
                };
                reader.readAsDataURL(file);
            }
        });
    } else {
        console.error(`Input or preview element not found: ${inputId}, ${previewId}`);
    }
}

/**
 * Display the forgery detection result
 * @param {Object} result - The detection result object
 */
function displayResult(result) {
    const resultContainer = document.getElementById('resultContainer');
    if (resultContainer) {
        resultContainer.classList.remove('hidden');
        resultContainer.innerHTML = `
            <p><strong>Forgery Detected:</strong> ${result.isForgery ? 'Yes' : 'No'}</p>
            <p><strong>Confidence:</strong> ${(result.confidence * 100).toFixed(2)}%</p>
            <p><strong>Good Matches:</strong> ${result.goodMatches}</p>
        `;
    } else {
        console.error('Result container not found');
    }
}
