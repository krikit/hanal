# -*- coding: utf-8 -*-


"""
TRIE library
"""
__author__ = 'krikit(krikit@naver.com)'
__copyright__ = 'Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License'


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
    :param  char:  character
    """
    self.depth = -1    # depth from root for flatten(serialize) nodes
    self.char = char    # character for key
    self.value = u''    # value
    self.children = {}    # child nodes
    self.child_start = -1    # number of nodes from this node to first child node

  def __unicode__(self):
    node_str = self.char if self.char else u'ROOT'
    if self.value:
      node_str = u'{%s: %s}' % (node_str, self.value)
    pfx = u''
    if self.depth > 0:
      pfx = u' ' * self.depth
    child_start_str = u''
    if self.child_start > 0:
      child_start_str = u', %d' % self.child_start
    return u'%s(%s, [%s]%s)' % (pfx, node_str, u', '.join(self.children.keys()), child_start_str)

  def __str__(self):
    return self.__unicode__().encode('UTF-8')


#############
# functions #
#############
def insert(root, key, val):
  """
  insert a key-value pair in node
  :param  root:  root node to insert
  :param  key:   key string
  :param  val:   value (string)
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
  :param    root:  root node start to find
  :param    key:   key string
  :return:         value (string), None is fail to find
  """
  node = root
  for char in key:
    if char not in node.children:
      return None
    else:
      node = node.children[char]
  return node.value


def breadth_first_traverse(root_node):
  """
  breadth first tranverse from given root node
  :param  root_node:  root node to traverse
  :return:            serialized list of nodes
  """
  def breadth_first_traverse_inner(idx, nodes):
    """
    breadth first traverse with depth information
    :param  idx:    current index
    :param  nodes:  nodes list
    """
    if idx >= len(nodes):
      return
    node = nodes[idx]
    logging.debug(u'TRAVERSE: %s', node)
    for key in sorted(node.children.keys()):
      child_node = node.children[key]
      child_node.depth = node.depth + 1
      nodes.append(child_node)
    breadth_first_traverse_inner(idx+1, nodes)

  def set_child_start(nodes):
    """
    set child_start field of nodes
    :param  nodes:  nodes list
    """
    partial_sum_of_children = 0
    num_of_next_siblings = 0
    for idx, node in enumerate(nodes):
      if idx == 0 or nodes[idx-1].depth != node.depth:
        partial_sum_of_children = 0
        num_of_next_siblings = 0
        for jdx in xrange(idx, len(nodes)):
          if nodes[jdx].depth == node.depth:
            num_of_next_siblings += 1
          else:
            break
      else:
        partial_sum_of_children += len(node.children)
        num_of_next_siblings -= 1
      node.child_start = partial_sum_of_children + num_of_next_siblings if node.children else -1

  root_node.depth = 0
  nodes = [root_node,]
  breadth_first_traverse_inner(0, nodes)
  set_child_start(nodes)
  return nodes
