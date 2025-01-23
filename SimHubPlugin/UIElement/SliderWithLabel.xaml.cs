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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace User.PluginSdkDemo.UIElement
{

    public partial class SliderWithLabel : UserControl
    {
        public SliderWithLabel()
        {
            InitializeComponent();
        }
        // Dependency Property for LabelText
        public static readonly DependencyProperty LabelTextProperty =
            DependencyProperty.Register(nameof(LabelText), typeof(string), typeof(SliderWithLabel),
                new PropertyMetadata("Default Label"));

        public string LabelText
        {
            get => (string)GetValue(LabelTextProperty);
            set => SetValue(LabelTextProperty, value);
        }

        // Dependency Property for SliderValue
        public static readonly DependencyProperty SliderValueProperty =
            DependencyProperty.Register(nameof(SliderValue), typeof(double), typeof(SliderWithLabel),
                new PropertyMetadata(0.0, OnSliderValueChanged));

        public double SliderValue
        {
            get => (double)GetValue(SliderValueProperty);
            set => SetValue(SliderValueProperty, value);
        }

        // Dependency Property for SliderMinimum
        public static readonly DependencyProperty SliderMinimumProperty =
            DependencyProperty.Register(nameof(SliderMinimum), typeof(double), typeof(SliderWithLabel),
                new PropertyMetadata(0.0));

        public double SliderMinimum
        {
            get => (double)GetValue(SliderMinimumProperty);
            set => SetValue(SliderMinimumProperty, value);
        }

        // Dependency Property for SliderMaximum
        public static readonly DependencyProperty SliderMaximumProperty =
            DependencyProperty.Register(nameof(SliderMaximum), typeof(double), typeof(SliderWithLabel),
                new PropertyMetadata(100.0));

        public double SliderMaximum
        {
            get => (double)GetValue(SliderMaximumProperty);
            set => SetValue(SliderMaximumProperty, value);
        }

        // Dependency Property for SliderTickFrequency
        public static readonly DependencyProperty SliderTickFrequencyProperty =
            DependencyProperty.Register(nameof(SliderTickFrequency), typeof(double), typeof(SliderWithLabel),
                new PropertyMetadata(1.0));

        public double SliderTickFrequency
        {
            get => (double)GetValue(SliderTickFrequencyProperty);
            set => SetValue(SliderTickFrequencyProperty, value);
        }

        // Event raised when the slider value changes
        public event RoutedPropertyChangedEventHandler<double> ValueChanged;

        private static void OnSliderValueChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var control = d as SliderWithLabel;
            control?.ValueChanged?.Invoke(control, new RoutedPropertyChangedEventArgs<double>(
                (double)e.OldValue, (double)e.NewValue));
        }

        private void Slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            SliderValue = e.NewValue; // Keep SliderValue in sync
        }
    }
}
