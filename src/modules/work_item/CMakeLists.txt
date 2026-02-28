px4_add_module(
	MODULE modules__work_item
	MAIN work_item_example
	SRCS
		WorkItemExample.cpp
	DEPENDS
		px4_work_queue
	MODULE_CONFIG
		work_item.yaml
	)
