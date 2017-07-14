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

namespace Pathfinding.WPFApplication
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void LoadGrid_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            dlg.DefaultExt = ".grid";
            dlg.Filter = "Grid Files (*.grid)|*.grid";
            bool? result = dlg.ShowDialog();
            if (result != true)
            {
                return;
            }

            string filename = dlg.FileName;
            MessageBox.Show(filename);

            PathGrid.Children.Clear();
            PathGrid.ColumnDefinitions.Clear();
            PathGrid.RowDefinitions.Clear();

            int gridWidth = new Random().Next(1, 11);
            int gridHeight = new Random().Next(1, 11);

            for (int y = 0; y < gridHeight; ++y)
            {
                RowDefinition row = new RowDefinition();
                row.Height = new GridLength(1, GridUnitType.Star);
                PathGrid.RowDefinitions.Add(row);
            }

            for (int x = 0; x < gridWidth; ++x)
            {
                ColumnDefinition col = new ColumnDefinition();
                col.Width = new GridLength(1, GridUnitType.Star);
                PathGrid.ColumnDefinitions.Add(col);
            }

            for (int y = 0; y < gridHeight; ++y)
            {
                for (int x = 0; x < gridWidth; ++x)
                {
                    Border panel = new Border();
                    Grid.SetColumn(panel, x);
                    Grid.SetRow(panel, y);

                    Label label = new Label();
                    label.Content = "(" + x + ", " + y + ")";
                    label.HorizontalAlignment = HorizontalAlignment.Center;
                    label.VerticalAlignment = VerticalAlignment.Center;
                    panel.Child = label;
                    panel.Margin = new Thickness(1);
                    panel.Background = new SolidColorBrush(Color.FromArgb(100, 100, 100, 255));
                    PathGrid.Children.Add(panel);
                }
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {

        }

        private void Algorithms_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }
    }
}
