/*
 *  Bifferboard RDC321x platform devices
 *
 *  Copyright (C) 2010 bifferos@yahoo.co.uk
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the
 *  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/mtd/physmap.h>
#include <linux/input.h>
#include <linux/slab.h>

#include <asm/rdc_boards.h>

static int __init
parse_bifferboard_partitions(struct mtd_info *master,
			     struct mtd_partition **pparts,
			     struct mtd_part_parser_data *plat_data)
{
	int res;
	size_t len;
	struct mtd_partition *rdc_flash_parts;
	u32 kernel_len;
	u16 tmp;

	res =  mtd_read(master, 0x4000 + 1036, 2, &len, (char *) &tmp);
	if (res)
		return res;
	kernel_len = tmp * master->erasesize;

	rdc_flash_parts = kzalloc(sizeof(struct mtd_partition) * 3, GFP_KERNEL);

	*pparts = rdc_flash_parts;

	rdc_flash_parts[0].name = "kernel";
	rdc_flash_parts[0].offset = 0;
	rdc_flash_parts[0].size = kernel_len;
	rdc_flash_parts[1].name = "rootfs";
	rdc_flash_parts[1].offset = kernel_len;
	rdc_flash_parts[1].size = master->size - kernel_len - 0x10000;
	rdc_flash_parts[2].name = "biffboot";
	rdc_flash_parts[2].offset = master->size - 0x10000;
	rdc_flash_parts[2].size = 0x10000;

	return 3;
}

struct mtd_part_parser __initdata bifferboard_parser = {
	.owner = THIS_MODULE,
	.parse_fn = parse_bifferboard_partitions,
	.name = "Bifferboard",
};

static int __init bifferboard_setup(void)
{
	return register_mtd_parser(&bifferboard_parser);
}
arch_initcall(bifferboard_setup);
