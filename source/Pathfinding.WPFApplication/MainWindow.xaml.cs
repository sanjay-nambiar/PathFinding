using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Pathfinding.WPFApplication
{
    public struct GridCell
    {
        public string glyph;
        public float heuristic;
        public float pathCost;
        public float totalCost;
    }

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        static Dictionary<string, Brush> Backgrounds = new Dictionary<string, Brush> {
            { "S", new SolidColorBrush(Color.FromArgb(255, 10, 10, 255)) },
            { "E", new SolidColorBrush(Color.FromArgb(255, 10, 255, 10)) },
            { "X", new SolidColorBrush(Color.FromArgb(255, 255, 255, 100)) },
            { "-", new SolidColorBrush(Color.FromArgb(255, 150, 150, 150 )) },
            { "|", new SolidColorBrush(Color.FromArgb(255, 30, 30, 30)) }
        };

        static List<string> PathFindingTypeNames = new List<string>() {
            "Breadth First",
            "Depth First",
            "Greedy Best First",
            "Dijkstra",
            "A*"
        };

        public string gridFile = "";

        public MainWindow()
        {
            InitializeComponent();
            Algorithms.ItemsSource = PathFindingTypeNames;
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

            gridFile = dlg.FileName;
            RunPathFindingForGrid(gridFile);
        }

        private void Algorithms_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (gridFile != "")
            {
                RunPathFindingForGrid(gridFile);
            }
        }

        private void RunPathFindingForGrid(string filename)
        {
            Process p = new Process();
            p.StartInfo.FileName = "Pathfinding.ConsoleApplication.exe";
            p.StartInfo.Arguments = filename + " 0 0 9 9 " + Algorithms.SelectedIndex;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardOutput = true;
            p.Start();

            char[] delimiter = { ' ' };
            List<List<GridCell>> outputGrid = new List<List<GridCell>>();
            string line = p.StandardOutput.ReadLine();
            while ((line = p.StandardOutput.ReadLine()) != null)
            {
                List<GridCell> row = new List<GridCell>();
                var cells = line.Split(delimiter, StringSplitOptions.RemoveEmptyEntries);
                if (cells.Length > 1)
                {
                    for (int cellId = 0; cellId < cells.Length; cellId += 4)
                    {
                        GridCell cell = new GridCell();
                        cell.glyph = cells[cellId];
                        cell.heuristic = float.Parse(cells[cellId + 1]);
                        cell.pathCost = float.Parse(cells[cellId + 2]);
                        cell.totalCost = float.Parse(cells[cellId + 3]);
                        row.Add(cell);
                    }
                    outputGrid.Add(row);
                }
                else
                {
                    RunStats.Content = "Time taken = " + float.Parse(cells[0]) + " micro seconds";
                }
            }
            p.WaitForExit();

            PathGrid.Children.Clear();
            PathGrid.ColumnDefinitions.Clear();
            PathGrid.RowDefinitions.Clear();

            Random random = new Random(DateTime.Now.Millisecond);
            int gridHeight = outputGrid.Count;
            int gridWidth = outputGrid[0].Count;

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

                    GridCell cell = outputGrid[y][x];
                    Label label = new Label();
                    label.Content = cell.heuristic + ", " + cell.pathCost + ", " + cell.totalCost;
                    label.HorizontalAlignment = HorizontalAlignment.Center;
                    label.VerticalAlignment = VerticalAlignment.Center;
                    panel.Child = label;
                    panel.Margin = new Thickness(1);
                    panel.Background = Backgrounds[cell.glyph];
                    PathGrid.Children.Add(panel);
                }
            }
        }
    }
}
