using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using System;
using System.IO;
using Tesseract;

namespace YourNamespace.Controllers
{
    [Route("api/image-to-text")]
    [ApiController]
    public class ImageToTextController : ControllerBase
    {
        [HttpPost]
        public ActionResult<string> ConvertImageToText(IFormFile imageFile)
        {
            try
            {
                using (var stream = new MemoryStream())
                {
                    imageFile.CopyTo(stream);
                    using (var engine = new TesseractEngine("./tessdata", "eng", EngineMode.Default))
                    {
                        using (var img = Pix.LoadFromMemory(stream.ToArray()))
                        {
                            using (var page = engine.Process(img))
                            {
                                var text = page.GetText();
                                return Ok(text);
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                return BadRequest($"Error: {ex.Message}");
            }
        }
    }
}
