using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ItemMaker
{
    class Armor:Equippable
    {
        public override void Load(System.IO.BinaryReader br)
        {
            base.Load(br);
        }

        public override void Save(System.IO.BinaryWriter bw)
        {
            base.Save(bw);
        }
    }
}
