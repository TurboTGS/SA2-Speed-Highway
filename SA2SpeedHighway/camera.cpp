#include "pch.h"


void FreeCam() {

	if (!isSADXLevel() || !MainCharObj1[0] || !MainCharObj2[0])
		return;

	if (CurrentCharacter == Characters_MechEggman || CurrentCharacter == Characters_MechTails)
	{
		CameraData[0].location.pos.y += ((MainCharObj2[0]->Speed.x) / 5);

		if (MainCharObj1[0]->Action == Action_Spring)
		{
			if (CameraData[0].location.pos.y <= (MainCharObj1[0]->Position.y - 12)) { CameraData[0].location.pos.y = (MainCharObj1[0]->Position.y - 12); }
			if (CameraData[0].location.pos.y >= (MainCharObj1[0]->Position.y + 12)) { CameraData[0].location.pos.y = (MainCharObj1[0]->Position.y + 12); }
		}
		if (MainCharObj1[0]->Action == Action_Fall) {
			CameraData[0].location.pos.y += ((MainCharObj2[0]->Speed.x) / 2);
		}
		if (MainCharObj1[0]->Action == Action_MechHover) {
			CameraData[0].location.pos.y += ((MainCharObj2[0]->Speed.x) / 2);
		}

		if (CameraData[0].location.pos.x >= (MainCharObj1[0]->Position.x + 90)) {
			CameraData[0].location.pos.x = (MainCharObj1[0]->Position.x + 90);
		}
		if (CameraData[0].location.pos.x <= (MainCharObj1[0]->Position.x - 90)) {
			CameraData[0].location.pos.x = (MainCharObj1[0]->Position.x - 90);
		}
		if (CameraData[0].location.pos.z >= (MainCharObj1[0]->Position.z + 90)) {
			CameraData[0].location.pos.z = (MainCharObj1[0]->Position.z + 90);
		}
		if (CameraData[0].location.pos.z <= (MainCharObj1[0]->Position.z - 90)) {
			CameraData[0].location.pos.z = (MainCharObj1[0]->Position.z - 90);
		}
		if (CameraData[0].location.pos.y >= (MainCharObj1[0]->Position.y + 70)) {
			CameraData[0].location.pos.y = (MainCharObj1[0]->Position.y + 70);
		}
		if (CameraData[0].location.pos.y <= (MainCharObj1[0]->Position.y + 10)) {
			CameraData[0].location.pos.y = (MainCharObj1[0]->Position.y + 10);
		}
	}
	else {

		if (MainCharObj1[0]->Action == Action_Fall) {
			CameraData[0].location.pos.y += ((MainCharObj2[0]->Speed.x) / 2);
		}

		if (MainCharObj1[0]->Action == Action_Spring)
		{
			if (CameraData[0].location.pos.y <= (MainCharObj1[0]->Position.y - 12))
				CameraData[0].location.pos.y = (MainCharObj1[0]->Position.y - 12);

			if (CameraData[0].location.pos.y >= (MainCharObj1[0]->Position.y + 12))
				CameraData[0].location.pos.y = (MainCharObj1[0]->Position.y + 12);
		}

		// keep cam close to player/prevent getting stuck
		if (CameraData[0].location.pos.x >= (MainCharObj1[0]->Position.x + 70)) {
			CameraData[0].location.pos.x = (MainCharObj1[0]->Position.x + 70);
		}
		if (CameraData[0].location.pos.x <= (MainCharObj1[0]->Position.x - 70)) {
			CameraData[0].location.pos.x = (MainCharObj1[0]->Position.x - 70);
		}
		if (CameraData[0].location.pos.z >= (MainCharObj1[0]->Position.z + 70)) {
			CameraData[0].location.pos.z = (MainCharObj1[0]->Position.z + 70);
		}
		if (CameraData[0].location.pos.z <= (MainCharObj1[0]->Position.z - 70)) {
			CameraData[0].location.pos.z = (MainCharObj1[0]->Position.z - 70);
		}

		if (
			((MainCharObj1[0]->Rotation.x >= 0x1000) && (MainCharObj1[0]->Rotation.x <= 0xF000)) ||
			((MainCharObj1[0]->Rotation.z >= 0x1000) && (MainCharObj1[0]->Rotation.z <= 0xF000))
			) // 90 - 270 degrees upside-down
		{
			// Z 8000 = upside down
			if (((MainCharObj1[0]->Rotation.x >= 0x3000) && (MainCharObj1[0]->Rotation.x <= 0xD000)) ||
				((MainCharObj1[0]->Rotation.z >= 0x3000) && (MainCharObj1[0]->Rotation.z <= 0xD000)))
			{
				if (((MainCharObj1[0]->Rotation.x >= 0x5000) && (MainCharObj1[0]->Rotation.x <= 0xB000)) ||
					((MainCharObj1[0]->Rotation.z >= 0x5000) && (MainCharObj1[0]->Rotation.z <= 0xB000)))
				{
					if (CameraData[0].location.pos.y <= (MainCharObj1[0]->Position.y - 70)) {
						CameraData[0].location.pos.y = (MainCharObj1[0]->Position.y - 70);
					}
					if (CameraData[0].location.pos.y >= (MainCharObj1[0]->Position.y - 10)) {
						CameraData[0].location.pos.y = (MainCharObj1[0]->Position.y - 10);
					}
					if (((MainCharObj1[0]->Rotation.x >= 0x7000) && (MainCharObj1[0]->Rotation.x <= 0x9000)) ||
						((MainCharObj1[0]->Rotation.z >= 0x7000) && (MainCharObj1[0]->Rotation.z <= 0x9000)))
					{
						CameraData[0].location.pos.y -= ((MainCharObj2[0]->Speed.x) / 3.3);
					}
					else { CameraData[0].location.pos.y -= ((MainCharObj2[0]->Speed.x) / 1.7); }
				}
			}
		}
		else
		{
			CameraData[0].location.pos.y += ((MainCharObj2[0]->Speed.x) / 3.3);

			if (CameraData[0].location.pos.y >= (MainCharObj1[0]->Position.y + 70)) {
				CameraData[0].location.pos.y = (MainCharObj1[0]->Position.y + 70);
			}

			if (CameraData[0].location.pos.y <= (MainCharObj1[0]->Position.y + 10)) {
				CameraData[0].location.pos.y = (MainCharObj1[0]->Position.y + 10);
			}
		}
	}
}