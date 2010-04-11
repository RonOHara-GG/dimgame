using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;


namespace EnemyMaker
{
    public partial class EnemyMaker : Form
    {
        class EnemyData
        {
            public int iNameLength;
            public string szName;
            public int iLevel;
            public int iHealth;
            public float fSpeed;

            public int iFileLength;
            public string szModel;

            public int iCrushRes;
            public int iSlashRes;
            public int iPierceRes;
            public int iMentalRes;
            public int iHeatRes;
            public int iColdRes;
            public int iElectRes;
            public int iWaterRes;
            public int iAcidRes;
            public int iViralRes;
            public int iHolyRes;
            public int iDeathRes;

            public int iDamage;
            public float fRange;
            public float fAttackSpeed;
            public int iDamageType;

        };

        private EnemyData currentData = null;

        public EnemyMaker()
        {
            InitializeComponent();
            currentData = new EnemyData();
        }

        private void TS_New_Click(object sender, EventArgs e)
        {
            if (currentData != null)
            {
                SetData();
                if (!PromptSave())
                    return;
            }

            currentData = new EnemyData();
            ResetFields();
        }

        private void TS_Open_Click(object sender, EventArgs e)
        {
            if (currentData != null)
            {
                SetData();
                if (!PromptSave())
                    return;
            }

            currentData = new EnemyData();
            LoadEnemy();
        }

        private void TS_Save_Click(object sender, EventArgs e)
        {
            if (currentData != null)
            {
                SetData();
                SaveEnemy();
            }

        }

        private void TS_Exit_Click(object sender, EventArgs e)
        {
            if (currentData != null)
            {
                SetData();

                if (!PromptSave())
                    return;
            }

            this.Close();

        }

        private void BTN_LoadModel_Click(object sender, EventArgs e)
        {
            if (currentData != null)
            {
                TXT_Model.Text = LoadModelDAE();
            }

        }

        private void SaveEnemy()
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "KP Enemy (*.kpe)|*.kpe|All Files (*.*)|*.*";

            if (dlg.ShowDialog() != DialogResult.Cancel)
            {
                //write file
                BinaryWriter bw = new BinaryWriter(new FileStream(dlg.FileName, FileMode.Create, FileAccess.Write));

                bw.Write(currentData.iNameLength);
                bw.Write(currentData.szName);
                bw.Write(currentData.iLevel);
                bw.Write(currentData.iHealth);
                bw.Write(currentData.fSpeed);

                bw.Write(currentData.iFileLength);
                bw.Write(currentData.szModel);

                bw.Write(currentData.iCrushRes);
                bw.Write(currentData.iSlashRes);
                bw.Write(currentData.iPierceRes);
                bw.Write(currentData.iMentalRes);
                bw.Write(currentData.iHeatRes);
                bw.Write(currentData.iColdRes);
                bw.Write(currentData.iElectRes);
                bw.Write(currentData.iWaterRes);
                bw.Write(currentData.iAcidRes);
                bw.Write(currentData.iViralRes);
                bw.Write(currentData.iHolyRes);
                bw.Write(currentData.iDeathRes);

                bw.Write(currentData.iDamage);
                bw.Write(currentData.fRange);
                bw.Write(currentData.fAttackSpeed);
                bw.Write(currentData.iDamageType);

                bw.Close();


            }
        }

        private void LoadEnemy()
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "KP Enemy (*.kpe)|*.kpe|All Files (*.*)|*.*";

            if (dlg.ShowDialog() != DialogResult.Cancel)
            {
                BinaryReader br = new BinaryReader(new FileStream(dlg.FileName, FileMode.Open, FileAccess.Read));

                currentData.iNameLength  = br.ReadInt32();
                currentData.szName = br.ReadString();
                currentData.iLevel  = br.ReadInt32();
                currentData.iHealth  = br.ReadInt32();
                currentData.fSpeed = br.ReadSingle();

                currentData.iFileLength  = br.ReadInt32();
                currentData.szModel = br.ReadString();

                currentData.iCrushRes  = br.ReadInt32();
                currentData.iSlashRes  = br.ReadInt32();
                currentData.iPierceRes  = br.ReadInt32();
                currentData.iMentalRes  = br.ReadInt32();
                currentData.iHeatRes  = br.ReadInt32();
                currentData.iColdRes  = br.ReadInt32();
                currentData.iElectRes  = br.ReadInt32();
                currentData.iWaterRes  = br.ReadInt32();
                currentData.iAcidRes  = br.ReadInt32();
                currentData.iViralRes  = br.ReadInt32();
                currentData.iHolyRes  = br.ReadInt32();
                currentData.iDeathRes  = br.ReadInt32();

                currentData.iDamage  = br.ReadInt32();
                currentData.fRange = br.ReadSingle();
                currentData.fAttackSpeed = br.ReadSingle();
                currentData.iDamageType  = br.ReadInt32();

                br.Close();

                SetFieldsWithData();
            }

        }

        private string LoadModelDAE()
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "DAE File (*.dae)|*.dae|All Files (*.*)|*.*";

            if (dlg.ShowDialog() != DialogResult.Cancel)
                return dlg.FileName;
            else
                return "";
        }

        private void SetData()
        {
            //Get basic data
            if (TXT_Name.Text != "")
            {
                currentData.szName = TXT_Name.Text;
                currentData.iNameLength = currentData.szName.Length;
            }
            else
            {
                currentData.szName = "No Name";
                currentData.iNameLength = currentData.szName.Length;
            }

            if (TXT_Level.Text != "")
                currentData.iLevel = int.Parse(TXT_Level.Text);
            else
                currentData.iLevel = 1;

            if (TXT_Health.Text != "")
                currentData.iHealth = int.Parse(TXT_Health.Text);
            else
                currentData.iHealth = 1;

            if (TXT_Speed.Text != "")
                currentData.fSpeed = float.Parse(TXT_Speed.Text);
            else
                currentData.fSpeed = 1;

            if (TXT_Model.Text != "")
            {
                currentData.szModel = TXT_Model.Text;
                currentData.iFileLength = currentData.szModel.Length;
            }
            else
            {
                currentData.szModel = "";
                currentData.iFileLength = currentData.szModel.Length;
            }

            //Store resist
            if (TXT_MentalResist.Text != "")
                currentData.iMentalRes = int.Parse(TXT_MentalResist.Text);
            else
                currentData.iMentalRes = 0;

            if (TXT_AcidResist.Text != "")
                currentData.iAcidRes = int.Parse(TXT_AcidResist.Text);
            else
                currentData.iAcidRes = 0;

            if (TXT_ColdResist.Text != "")
                currentData.iColdRes = int.Parse(TXT_ColdResist.Text);
            else
                currentData.iColdRes = 0;

            if (TXT_CrushResist.Text != "")
                currentData.iCrushRes = int.Parse(TXT_CrushResist.Text);
            else
                currentData.iCrushRes = 0;

            if (TXT_DeathResist.Text != "")
                currentData.iDeathRes = int.Parse(TXT_DeathResist.Text);
            else
                currentData.iDeathRes = 0;

            if (TXT_ElectricResist.Text != "")
                currentData.iElectRes = int.Parse(TXT_ElectricResist.Text);
            else
                currentData.iElectRes = 0;

            if (TXT_HeatResist.Text != "")
                currentData.iHeatRes = int.Parse(TXT_HeatResist.Text);
            else
                currentData.iHeatRes = 0;

            if (TXT_HolyResist.Text != "")
                currentData.iHolyRes = int.Parse(TXT_HolyResist.Text);
            else
                currentData.iHolyRes = 0;

            if (TXT_PierceResist.Text != "")
                currentData.iPierceRes = int.Parse(TXT_PierceResist.Text);
            else
                currentData.iPierceRes = 0;

            if (TXT_SlashResist.Text != "")
                currentData.iSlashRes = int.Parse(TXT_SlashResist.Text);
            else
                currentData.iSlashRes = 0;

            if (TXT_ViralResist.Text != "")
                currentData.iViralRes = int.Parse(TXT_ViralResist.Text);
            else
                currentData.iViralRes = 0;

            if (TXT_WaterResist.Text != "")
                currentData.iWaterRes = int.Parse(TXT_WaterResist.Text);
            else
                currentData.iWaterRes = 0;

            //store combat data
            if (TXT_Damge.Text != "")
                currentData.iDamage = int.Parse(TXT_Damge.Text);
            else
                currentData.iDamage = 1;

            if (TXT_Range.Text != "")
                currentData.fRange = float.Parse(TXT_Range.Text);
            else
                currentData.fRange = 1;

            if (TXT_AttackSpeed.Text != "")
                currentData.fAttackSpeed = float.Parse(TXT_AttackSpeed.Text);
            else
                currentData.fAttackSpeed = 1.0f;

            if (CB_DamageType.SelectedIndex > -1)
                currentData.iDamageType = CB_DamageType.SelectedIndex;
            else
                currentData.iDamageType = 0;


        }

        private void SetFieldsWithData()
        {
            TXT_Name.Text = currentData.szName;

            TXT_Level.Text = currentData.iLevel.ToString();

            TXT_Health.Text = currentData.iHealth.ToString();

            TXT_Speed.Text = currentData.fSpeed.ToString();

            TXT_Model.Text = currentData.szModel;

            TXT_MentalResist.Text = currentData.iMentalRes.ToString();

            TXT_AcidResist.Text = currentData.iAcidRes.ToString();

            TXT_ColdResist.Text = currentData.iColdRes.ToString();

            TXT_CrushResist.Text = currentData.iCrushRes.ToString();

            TXT_DeathResist.Text = currentData.iDeathRes.ToString();

            TXT_ElectricResist.Text = currentData.iElectRes.ToString();

            TXT_HeatResist.Text = currentData.iHeatRes.ToString();

            TXT_HolyResist.Text = currentData.iHolyRes.ToString();

            TXT_PierceResist.Text = currentData.iPierceRes.ToString();

            TXT_SlashResist.Text = currentData.iSlashRes.ToString();

            TXT_ViralResist.Text = currentData.iViralRes.ToString();

            TXT_WaterResist.Text = currentData.iWaterRes.ToString();

            TXT_Damge.Text = currentData.iDamage.ToString();

            TXT_Range.Text = currentData.fRange.ToString();

            TXT_AttackSpeed.Text = currentData.fAttackSpeed.ToString();

            CB_DamageType.SelectedIndex = currentData.iDamageType;


        }

        private void ResetFields()
        {
            TXT_Name.Text = "";

            TXT_Level.Text = "";

            TXT_Health.Text = "";

            TXT_Speed.Text = "";

            TXT_Model.Text = "";

            TXT_MentalResist.Text = "";

            TXT_AcidResist.Text = "";

            TXT_ColdResist.Text = "";

            TXT_CrushResist.Text = "";

            TXT_DeathResist.Text = "";

            TXT_ElectricResist.Text = "";

            TXT_HeatResist.Text = "";

            TXT_HolyResist.Text = "";

            TXT_PierceResist.Text = "";

            TXT_SlashResist.Text = "";

            TXT_ViralResist.Text = "";

            TXT_WaterResist.Text = "";

            TXT_Damge.Text = "";

            TXT_Range.Text = "";

            TXT_AttackSpeed.Text = "";

            CB_DamageType.SelectedIndex = 0;

        }

        private bool PromptSave()
        {
            DialogResult dlg = MessageBox.Show("Save current enemy?", "", MessageBoxButtons.YesNoCancel);

            if (dlg == DialogResult.Yes)
            {
                SaveEnemy();
                return true;
            }
            else if (dlg == DialogResult.No)
                return true;
            else
                return false;
        }

    }
}
