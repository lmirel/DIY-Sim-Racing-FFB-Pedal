using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Newtonsoft.Json;
using System.Net.Http;



namespace User.PluginSdkDemo
{

    public partial class OnlineProfile : Window
    {
        public string SelectedFileName { get; private set; }
        public OnlineProfile()
        {
            InitializeComponent();
            LoadProfiles();
        }

        private async void LoadProfiles()
        {
            string jsonUrl = "https://raw.githubusercontent.com/tcfshcrw/FFB_PEDAL_PROFILE/master/main.json";

            using (HttpClient client = new HttpClient())
            {
                string jsonString = await client.GetStringAsync(jsonUrl);
                var profilesData = JsonConvert.DeserializeObject<ProfilesData>(jsonString);

                ProfilesListBox.ItemsSource = profilesData.Profiles;
                ProfilesListBox.DisplayMemberPath = "ProfileName";
            }
        }

        private void CloseAndReturn_Click(object sender, RoutedEventArgs e)
        {
            if (ProfilesListBox.SelectedItem is Profile selectedProfile)
            {
                SelectedFileName = selectedProfile.FileName;
                DialogResult = true;
            }
            else
            {
                MessageBox.Show("Please select a profile.");
            }
        }

        private void ProfilesListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (ProfilesListBox.SelectedItem is Profile selectedProfile)
            {
                Label_Online_Profile_Description.Content = "Author: "+selectedProfile.Author+"\nVersion: "+selectedProfile.Version+"\n"+selectedProfile.Description;
            }
            
        }


        private void Btn_Online_profile_leave_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
        }
    }

    public class ProfilesData
    {
        public List<Profile> Profiles { get; set; }
    }

    public class Profile
    {
        public string Author { get; set; }
        public string ProfileName { get; set; }
        public string Version { get; set; }
        public string FileName { get; set; }
        public string Description { get; set; }
    }
}
