import requests
import re

# URL to get releases from the GitHub repository
repo_url = "https://api.github.com/repos/ChrGri/DIY-Sim-Racing-FFB-Pedal/releases"

# Send a GET request to the GitHub API through the proxy
response = requests.get(repo_url)

# Prepare the output message
output_message = ""

# Check if the request was successful (status code 200)
if response.status_code == 200:
    releases = response.json()  # Parse the JSON response

    # Filter for non-prerelease versions (i.e., where "prerelease" is False)
    non_prerelease_releases = [release for release in releases if not release['prerelease']]

    if non_prerelease_releases:
        # Sort the non-prerelease releases by their "created_at" field (in descending order)
        sorted_releases = sorted(non_prerelease_releases, key=lambda x: x['created_at'], reverse=True)

        # The first release in the sorted list will be the most recent non-prerelease
        latest_release = sorted_releases[0]
        version_number = latest_release['tag_name'][17:]  # The tag name is usually the version number

        tag_name = latest_release['tag_name']
        # Use regex to extract the number after the last underscore
        match = re.search(r'Release_Package_v(\d+)', tag_name)
        if match:
            version_number = float(match.group(1))  # Convert the extracted version to an integer
            new_version_number = version_number + .1  # Increment the version number by 1
            #output_message = f"xyz_v{new_version_number}"
            output_message = str(new_version_number)
        else:
            output_message = "0"
            

        #output_message = f"The version number of the latest non-prerelease release is: {version_number}"
        #output_message = str( float(version_number) + 0.1 ) # increment the version number
    else:
        output_message = "No non-prerelease releases found."
else:
    output_message = f"Failed to fetch releases. HTTP Status Code: {response.status_code}"

# Write the result to a file
file_path = "expectedVersion.txt"

with open(file_path, "w") as file:
    file.write(output_message)

print(f"Output written to {file_path}")

print("Version is: " + output_message)
