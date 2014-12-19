# -*- coding: utf-8 -*-


"""TRIE library"""
__author__ = 'krikit'
__email__ = 'krikit@naver.com'
__date__ = 'creation: 2014-12-19, modification: 2014-12-19'
__copyright__ = 'Copyright (C) 2014, krikit. All rights reserved. BSD 2-Clause License'


###########
# imports #
###########
import logging


#########
# types #
#########
class Node(object):
  """
  TRIE nodes
  """
  def __init__(self, char=u''):
    """
    @param  char  character
    """
    self.char = char
    self.value = u''
    self.children = {}

  def __unicode__(self):
    node_str = self.char if self.char else u'ROOT'
    if self.value:
      node_str = u'{%s: %s}' % (node_str, self.value)
    return u'(%s, [%s])' % (node_str, u', '.join(self.children.keys()))

  def __str__(self):
    return self.__unicode__().encode('UTF-8')


#############
# functions #
#############
def insert(root, key, val):
  """
  insert a key-value pair in node
  @param  root  root node to insert
  @param  key   key string
  @param  val   value (string)
  """
  if not key:
    root.value = val
  elif key[0] in root.children:
    insert(root.children[key[0]], key[1:], val)
  else:
    new_node = Node(key[0])
    root.children[key[0]] = new_node
    insert(new_node, key[1:], val)
  logging.debug(u'INSERT {%s: %s} INTO %s', key, val, root)


def find(root, key):
  """
  find value with key
  @param  root  root node start to find
  @param  key   key string
  @return       value (string), None is fail to find
  """
  node = root
  for char in key:
    if char not in node.children:
      return None
    else:
      node = node.children[char]
  return node.value
