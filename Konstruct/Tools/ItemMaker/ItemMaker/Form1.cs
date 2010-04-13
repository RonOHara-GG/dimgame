using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.IO;

namespace ItemMaker
{
    public partial class ItemMaker : Form
    {
        private ArrayList groupBoxes = new ArrayList();

        private Item currentItem = null;

        private enum ItemType
        {
            eIT_None,
            eIT_Item,
            eIT_Weapon,
            eIT_Armor
        };

        ItemType m_eCurrentItemType;

        public ItemMaker()
        {
            InitializeComponent();
            m_eCurrentItemType = ItemType.eIT_None;
        }

        private void TS_Save_Click(object sender, EventArgs e)
        {

        }

        private void TS_Exit_Click(object sender, EventArgs e)
        {

        }

        private void TS_NewArmor_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Implement Me!");
        }

        private void TS_NewWeapon_Click(object sender, EventArgs e)
        {
            if (m_eCurrentItemType != ItemType.eIT_None)
            {
                PromptSave();

                m_eCurrentItemType = ItemType.eIT_Weapon;
                ShowWeaponFields();
                ResetWeaponFeilds();
            }
            

        }

        private void TS_Open_Click(object sender, EventArgs e)
        {

        }

        private void SaveItem()
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "KP Item (*.kpi)|*.kpi|All Files (*.*)|*.*";

            if (dlg.ShowDialog() != DialogResult.Cancel)
            {
                //write file
                BinaryWriter bw = new BinaryWriter(new FileStream(dlg.FileName, FileMode.Create, FileAccess.Write));

                           

                bw.Close();
            }
        }

        private void LoadItem()
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "KP Item (*.kpi)|*.kpi|All Files (*.*)|*.*";

            if (dlg.ShowDialog() != DialogResult.Cancel)
            {
                BinaryReader br = new BinaryReader(new FileStream(dlg.FileName, FileMode.Open, FileAccess.Read));

                currentItem.Load(br);
                

                br.Close();

                
            }

        }


        private bool PromptSave()
        {
            DialogResult dlg = MessageBox.Show("Save current item?", "", MessageBoxButtons.YesNoCancel);

            if (dlg == DialogResult.Yes)
            {
                SaveItem();
                return true;
            }
            else if (dlg == DialogResult.No)
                return true;
            else
                return false;
        }

        private void ShowWeaponFields()
        {
            CloseAllFields();

            EquippableGroup.Visible = true;
            WeaponGroup.Visible = true;
        }

        private void ResetWeaponFeilds()
        {
            ResetEquippableFields();

            for (int i = 0; i < EquippableGroup.Controls.Count; i++)
            {
                Control control = (Control)EquippableGroup.Controls[i];

                if (control is TextBox)
                {
                    TextBox tb = (TextBox)control;

                    tb.Text = "";
                }
                else if (control is ComboBox)
                {
                    ((ComboBox)control).SelectedIndex = 0;

                }


            }
        }

        private void ResetEquippableFields()
        {
            for (int i = 0; i < EquippableGroup.Controls.Count; i++)
            {
                Control control = (Control)EquippableGroup.Controls[i];

                if (control is TextBox)
                {
                    TextBox tb = (TextBox)control;
                    tb.Text = "";
                }              


            }
            

        }

        private void CloseAllFields()
        {
            //close all group boxes
            for(int i = 0; i < groupBoxes.Count; i++)
            {
                GroupBox box1 = (GroupBox)groupBoxes[i];
                box1.Visible = false;
            }
        }

        private void Save(BinaryWriter bw)
        {
            switch (m_eCurrentItemType)
            {
                case ItemType.eIT_Weapon:
                    {
                        bw.Write((int)ItemType.eIT_Weapon);

                        for (int i = 0; i < EquippableGroup.Controls.Count; i++)
                        {
                            Control control = (Control)EquippableGroup.Controls[i];

                            if (control is TextBox)
                            {
                                TextBox tb = (TextBox)control;

                                if (tb.Text != "")
                                {
                                    float num = 0;

                                    if (float.TryParse(tb.Text, out num))
                                        bw.Write(num);
                                    else
                                        bw.Write(tb.Text);
                                }
                                else
                                    bw.Write(0);
                            }
                            else if(control is ComboBox)
                            {
                                bw.Write(((ComboBox)control).SelectedIndex);
                               
                            }
                            
                        
                        }

                        for (int i = 0; i < WeaponGroup.Controls.Count; i++)
                        {
                            Control control = (Control)EquippableGroup.Controls[i];

                            if (control is TextBox)
                            {
                                TextBox tb = (TextBox)control;

                                if (tb.Text != "")
                                {
                                    float num = 0;

                                    if (float.TryParse(tb.Text, out num))
                                        bw.Write(num);
                                    else
                                        bw.Write(tb.Text);
                                }
                                else
                                    bw.Write(0);
                            }
                            else if (control is ComboBox)
                            {
                                bw.Write(((ComboBox)control).SelectedIndex);

                            }


                        }
                        break;
                    }
            }
        }

        private void Load(BinaryReader br)
        {
            int iType = br.ReadInt32();

            switch ((ItemType)iType)
            {
                case ItemType.eIT_Weapon:
                    {

                        for (int i = 0; i < EquippableGroup.Controls.Count; i++)
                        {
                            Control control = (Control)EquippableGroup.Controls[i];

                            if (control is TextBox)
                            {
                                TextBox tb = (TextBox)control;

                                
                                
                            }
                            else if (control is ComboBox)
                            {
                                bw.Write(((ComboBox)control).SelectedIndex);

                            }


                        }

                        for (int i = 0; i < WeaponGroup.Controls.Count; i++)
                        {
                            Control control = (Control)EquippableGroup.Controls[i];

                            if (control is TextBox)
                            {
                                TextBox tb = (TextBox)control;

                                if (tb.Text != "")
                                {
                                    float num = 0;

                                    if (float.TryParse(tb.Text, out num))
                                        bw.Write(num);
                                    else
                                        bw.Write(tb.Text);
                                }
                                else
                                    bw.Write(0);
                            }
                            else if (control is ComboBox)
                            {
                                bw.Write(((ComboBox)control).SelectedIndex);

                            }


                        }
                        break;
                    }
            }



        }




    }
}
