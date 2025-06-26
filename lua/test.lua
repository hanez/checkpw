#!/usr/bin/env lua

-- This file is part of chkgrp that is a program that checks if a user is a
-- member of a group.
--
-- Author: Johannes Findeisen <you@hanez.org> - 2025
-- Homepage: https://git.xw3.org/hanez/chkgrp
-- License: Apache-2.0 (see LICENSE)

local checkgroup = require("checkgroup")

local user = arg[1]
local group = arg[2]

local result = checkgroup.is_user_in_group(user, group)

if result == true then
    print("Yes")
    os.exit(0)
elseif result == false then
    print("No")
    os.exit(1)
else
    os.exit(2)
end

