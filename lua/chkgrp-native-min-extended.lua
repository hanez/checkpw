#!/usr/bin/env lua

-- This file is part of chkgrp that is a program that checks if a user is a
-- member of a group.
--
-- Author: Johannes Findeisen <you@hanez.org> - 2025
-- Homepage: https://git.xw3.org/hanez/chkgrp
-- License: Apache-2.0 (see LICENSE)

local MAX_NAME = 256

local function read_passwd_gid(username)
    local f = io.open("/etc/passwd", "r")
    if not f then return nil end

    for line in f:lines() do
        local fields = {}
        for field in string.gmatch(line, "([^:]+)") do
            table.insert(fields, field)
        end
        if fields[1] == username then
            f:close()
            return tonumber(fields[4]) -- GID is 4th field
        end
    end

    f:close()
    return nil
end

local function check_group_membership(username, groupname, user_gid)
    local f = io.open("/etc/group", "r")
    if not f then return nil end

    for line in f:lines() do
        local fields = {}
        for field in string.gmatch(line, "([^:]+)") do
            table.insert(fields, field)
        end
        if fields[1] == groupname then
            local group_gid = tonumber(fields[3])
            if group_gid == user_gid then
                f:close()
                return true
            end

            for member in string.gmatch(fields[4] or "", "[^,]+") do
                if member == username then
                    f:close()
                    return true
                end
            end

            f:close()
            return false
        end
    end

    f:close()
    return nil
end

local username = arg[1]
local groupname = arg[2]

if not username or not groupname then
    os.exit(2)
end

if #username > MAX_NAME or #groupname > MAX_NAME then
    os.exit(2)
end

local user_gid = read_passwd_gid(username)
if not user_gid then
    os.exit(2)
end

local result = check_group_membership(username, groupname, user_gid)
if result == nil then
    os.exit(2)
elseif result == true then
    print("Yes")
    os.exit(0)
else
    print("No")
    os.exit(1)
end

