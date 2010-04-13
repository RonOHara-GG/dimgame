using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Collections;
using System.Windows.Forms;

namespace ItemMaker
{
    class Item
    {
        protected string m_szName;

        public enum ItemType
        {
            e_Item,
            e_Weapon,
            e_Armor
        };

        protected ItemType m_eType = ItemType.e_Item;    

        public virtual void Save(BinaryWriter bw)
        {
            bw.Write((int)m_eType);
            bw.Write(m_szName.Length);
            bw.Write(m_szName);

        }

        public virtual void Load(BinaryReader br)
        {
            br.ReadInt32();
            m_szName = br.ReadString();
        }

        public virtual void SetData(ArrayList textBoxes)
        {            
            TextBox box = (TextBox)textBoxes[0];
            m_szName = box.Text;
        }

        public virtual void SetFields(ArrayList textBoxes)
        {
            ((TextBox)textBoxes[0]).Text = m_szName;
        }
    }
}
