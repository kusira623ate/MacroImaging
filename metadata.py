import piexif
import glob

def add_focal_length_to_image(image_path, focal_length_mm):
    # Define the focal length in EXIF format
    focal_length_exif = (int(focal_length_mm * 100), 100)  # Convert to EXIF format (numerator, denominator)
    
    # Create the EXIF data dictionary
    exif_dict = {
        "Exif": {
            piexif.ExifIFD.FocalLength: focal_length_exif
        }
    }

    # Add the EXIF data to the existing image
    exif_bytes = piexif.dump(exif_dict)
    piexif.insert(exif_bytes, image_path)
    print(f'Added focal length metadata to: {image_path}')

# Path to your images
image_folder = 'C:/Users/kusha/OneDrive/Desktop/Sample_Meshroom/62'  # Replace with your folder path
focal_length_mm = 35  # Focal length in mm

# Process all images in the folder
for image_path in glob.glob(f"{image_folder}/*.jpg"):
    add_focal_length_to_image(image_path, focal_length_mm)
