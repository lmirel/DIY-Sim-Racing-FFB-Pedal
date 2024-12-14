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
using System.ComponentModel;



namespace User.PluginSdkDemo
{

    public partial class OnlineProfile : Window
    {
        public string SelectedFileName { get; private set; }
        private DAP_config_st tmp_config;
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
                CollectionView view = (CollectionView)CollectionViewSource.GetDefaultView(ProfilesListBox.ItemsSource);
                view.SortDescriptions.Add(new SortDescription("ProfileName", ListSortDirection.Ascending));
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

        private async void ProfilesListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (ProfilesListBox.SelectedItem is Profile selectedProfile)
            {
                Label_Online_Profile_Description.Content = "Author: "+selectedProfile.Author+"\nVersion: "+selectedProfile.Version+"\n"+selectedProfile.Description;
                //Label_Online_Profile_Description.Content = "\n URL:" + selectedProfile.FileName;
                try
                {
                    string jsonUrl = "https://raw.githubusercontent.com/tcfshcrw/FFB_PEDAL_PROFILE/master/Profiles/" + selectedProfile.FileName;
                    tmp_config = await GetProfileDataAsync(jsonUrl);
                    Update_ForceCurve();
                    
                }
                catch (Exception ex)
                {
                    System.Windows.MessageBox.Show($"Error loading JSON: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Warning);

                }
                

            }
            
        }


        private void Btn_Online_profile_leave_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
        }

        private async Task<DAP_config_st> GetProfileDataAsync(string url)
        {
            using (HttpClient client = new HttpClient())
            {
                string jsonString = await client.GetStringAsync(url);
                //return JsonConvert.DeserializeObject<Profile_Online>(jsonString);
                return JsonConvert.DeserializeObject<DAP_config_st>(jsonString);
            }
        }
        private void Update_ForceCurve()
        {

            double[] x = new double[6];
            double[] y = new double[6];
            double x_quantity = 100;
            double y_max = 100;
            double dx = canvas_Online_Config_curve.Width / x_quantity;
            double dy = canvas_Online_Config_curve.Height / y_max;
            //draw pedal force-travel curve
            x[0] = 0;
            x[1] = 20;
            x[2] = 40;
            x[3] = 60;
            x[4] = 80;
            x[5] = 100;

            y[0] = tmp_config.payloadPedalConfig_.relativeForce_p000;
            y[1] = tmp_config.payloadPedalConfig_.relativeForce_p020;
            y[2] = tmp_config.payloadPedalConfig_.relativeForce_p040;
            y[3] = tmp_config.payloadPedalConfig_.relativeForce_p060;
            y[4] = tmp_config.payloadPedalConfig_.relativeForce_p080;
            y[5] = tmp_config.payloadPedalConfig_.relativeForce_p100;

            // Use cubic interpolation to smooth the original data
            (double[] xs2, double[] ys2, double[] a, double[] b) = Cubic.Interpolate1D(x, y, 100);
            System.Windows.Media.PointCollection myPointCollection2 = new System.Windows.Media.PointCollection();


            for (int pointIdx = 0; pointIdx < 100; pointIdx++)
            {
                System.Windows.Point Pointlcl = new System.Windows.Point(dx * xs2[pointIdx], dy * ys2[pointIdx]);
                myPointCollection2.Add(Pointlcl);
                
            }
            this.Polyline_Online_Config_ForceCurve.Points = myPointCollection2;
            //set the rect
            double control_rect_value_max = 100;
            double dyy = canvas_Online_Config_curve.Height / control_rect_value_max;
            Canvas.SetTop(rect0_Online_Config, canvas_Online_Config_curve.Height - dyy * tmp_config.payloadPedalConfig_.relativeForce_p000 - rect0_Online_Config.Height / 2);
            Canvas.SetLeft(rect0_Online_Config, 0 * canvas_Online_Config_curve.Width / 5 - rect0_Online_Config.Width / 2);
            Canvas.SetTop(rect1_Online_Config, canvas_Online_Config_curve.Height - dyy * tmp_config.payloadPedalConfig_.relativeForce_p020 - rect1_Online_Config.Height / 2);
            Canvas.SetLeft(rect1_Online_Config, 1 * canvas_Online_Config_curve.Width / 5 - rect1_Online_Config.Width / 2);
            Canvas.SetTop(rect2_Online_Config, canvas_Online_Config_curve.Height - dyy * tmp_config.payloadPedalConfig_.relativeForce_p040 - rect2_Online_Config.Height / 2);
            Canvas.SetLeft(rect2_Online_Config, 2 * canvas_Online_Config_curve.Width / 5 - rect2_Online_Config.Width / 2);
            Canvas.SetTop(rect3_Online_Config, canvas_Online_Config_curve.Height - dyy * tmp_config.payloadPedalConfig_.relativeForce_p060 - rect3_Online_Config.Height / 2);
            Canvas.SetLeft(rect3_Online_Config, 3 * canvas_Online_Config_curve.Width / 5 - rect3_Online_Config.Width / 2);
            Canvas.SetTop(rect4_Online_Config, canvas_Online_Config_curve.Height - dyy * tmp_config.payloadPedalConfig_.relativeForce_p080 - rect4_Online_Config.Height / 2);
            Canvas.SetLeft(rect4_Online_Config, 4 * canvas_Online_Config_curve.Width / 5 - rect4_Online_Config.Width / 2);
            Canvas.SetTop(rect5_Online_Config, canvas_Online_Config_curve.Height - dyy * tmp_config.payloadPedalConfig_.relativeForce_p100 - rect5_Online_Config.Height / 2);
            Canvas.SetLeft(rect5_Online_Config, 5 * canvas_Online_Config_curve.Width / 5 - rect5_Online_Config.Width / 2);
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
