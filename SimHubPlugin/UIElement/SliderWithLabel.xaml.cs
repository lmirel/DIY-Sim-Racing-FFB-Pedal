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
        // Dependency Property for SliderName
        public static readonly DependencyProperty SliderNameProperty =
            DependencyProperty.Register(nameof(SliderName), typeof(string), typeof(SliderWithLabel),
                new PropertyMetadata("Slider"));

        public string SliderName
        {
            get => (string)GetValue(SliderNameProperty);
            set => SetValue(SliderNameProperty, value);
        }

        // Dependency Property for Unit
        public static readonly DependencyProperty UnitProperty =
            DependencyProperty.Register(nameof(Unit), typeof(string), typeof(SliderWithLabel),
                new PropertyMetadata(""));

        public string Unit
        {
            get => (string)GetValue(UnitProperty);
            set => SetValue(UnitProperty, value);
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

        // Dependency Property for MinValue
        public static readonly DependencyProperty MinValueProperty =
            DependencyProperty.Register(nameof(MinValue), typeof(double), typeof(SliderWithLabel),
                new PropertyMetadata(0.0));

        public double MinValue
        {
            get => (double)GetValue(MinValueProperty);
            set => SetValue(MinValueProperty, value);
        }

        // Dependency Property for MaxValue
        public static readonly DependencyProperty MaxValueProperty =
            DependencyProperty.Register(nameof(MaxValue), typeof(double), typeof(SliderWithLabel),
                new PropertyMetadata(100.0));

        public double MaxValue
        {
            get => (double)GetValue(MaxValueProperty);
            set => SetValue(MaxValueProperty, value);
        }

        // Dependency Property for TickFrequency
        public static readonly DependencyProperty TickFrequencyProperty =
            DependencyProperty.Register(nameof(TickFrequency), typeof(double), typeof(SliderWithLabel),
                new PropertyMetadata(1.0));

        public double TickFrequency
        {
            get => (double)GetValue(TickFrequencyProperty);
            set => SetValue(TickFrequencyProperty, value);
        }

        // Dependency Property for SliderLength
        public static readonly DependencyProperty SliderLengthProperty =
            DependencyProperty.Register(nameof(SliderLength), typeof(double), typeof(SliderWithLabel),
                new PropertyMetadata(400.0));

        public double SliderLength
        {
            get => (double)GetValue(SliderLengthProperty);
            set => SetValue(SliderLengthProperty, value);
        }
        // Dependency Property for LabelContent
        public static readonly DependencyProperty LabelContentProperty =
            DependencyProperty.Register(nameof(LabelContent), typeof(string), typeof(SliderWithLabel),
                new PropertyMetadata("Slider: 0"));

        public string LabelContent
        {
            get => (string)GetValue(LabelContentProperty);
            private set => SetValue(LabelContentProperty, value);
        }

        // Event to notify the main window of slider value changes
        public event RoutedPropertyChangedEventHandler<double> SliderValueChanged;

        private static void OnSliderValueChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is SliderWithLabel control)
            {
                control.UpdateLabelContent();
                control.SliderValueChanged?.Invoke(control,
                    new RoutedPropertyChangedEventArgs<double>((double)e.OldValue, (double)e.NewValue));
            }
        }

        private void SliderElement_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            UpdateLabelContent();
        }

        private void UpdateLabelContent()
        {
            LabelContent = $"{SliderName}: {SliderValue}{Unit}";
        }
    }
}
